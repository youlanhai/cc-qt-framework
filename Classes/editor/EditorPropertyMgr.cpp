//
//  EditorValueTypeMgr.cpp
//  Editor
//
//  Created by C218-pc on 15/7/9.
//  Copyright (c) 2015年 Bullets in a Burning Box, Inc. All rights reserved.
//

#include "EditorPropertyMgr.h"
#include "FileTools.h"
#include "UIHelper.h"

#include "EditorClassNameMap.h"
#include "EditorPropertyUI.h"
#include "EditorPropertyUIFactory.h"
#include "LogTool.h"

DEFINE_LOG_COMPONENT(LOG_PRIORITY_DEBUG, "Property");
IMPLEMENT_SINGLETON(Editor::PropertyMgr);

namespace Editor
{
    
    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    PropertyNode::PropertyNode()
        : m_valueRoot(NULL)
        , m_parent(NULL)
        , m_propertyUI(NULL)
    {  
    }
    
    PropertyNode::~PropertyNode()
    {
        CC_SAFE_DELETE(m_valueRoot);
        CC_SAFE_RELEASE(m_propertyUI);
    }
    
    bool PropertyNode::load(rapidjson::Value & config)
    {
        rapidjson::Value *value = &config["name"];
        if(!value->IsString())
        {
            return false;
        }
        m_name = value->GetString();
        
        value = &config["parent"];
        if(value->IsString())
        {
            m_parent = PropertyMgr::instance()->findProperty(value->GetString());
            if(NULL == m_parent)
            {
                LOG_ERROR("Failed to find parent '%s' for type '%s'",
                           value->GetString(), m_name.c_str());
                return false;
            }
        }
        
        value = &config["property"];
        if(!value->IsObject())
        {
            return false;
        }
        
        m_valueRoot = new PropertyTypedef();
        m_valueRoot->m_type = "class";
        m_valueRoot->m_key = m_name;
        m_valueRoot->m_name = m_name;
        
        if(!m_valueRoot->loadChildren(*value))
            return false;
        
        m_propertyUI = PropertyUIFactory::instance()->createPropertyByType(m_valueRoot);
        if(NULL == m_propertyUI)
        {
            LOG_ERROR("Failed to create ui for property '%s'", m_name.c_str());
            return false;
        }
        
        m_propertyUI->retain();
        return true;
    }
    
    //////////////////////////////////////////////////////////////////
    ///
    //////////////////////////////////////////////////////////////////
    
    PropertyMgr::PropertyMgr()
    {
        loadDefaultClassNameMap(this);
    }
    
    PropertyMgr::~PropertyMgr()
    {
        
    }
    
    bool PropertyMgr::loadPropertyFile(const std::string & filename)
    {
        rapidjson::Document document;
        if(!openJsonFile(filename, document) || !document.IsArray())
        {
            return false;
        }
        
        std::string path = filename;
        size_t split = path.rfind('/');
        if(split != path.npos)
        {
            path.erase(split + 1);
        }
        
        for(rapidjson::Value::ValueIterator it = document.Begin();
            it != document.End(); ++it)
        {
            if(it->IsString())
            {
                if(!loadPropertyFile(path + it->GetString()))
                {
                    return false;
                }
            }
            else if(it->IsObject())
            {
                if(!registerProperty(*it))
                {
                    return false;
                }
            }
        }
        return true;
    }
    
    PropertyNode* PropertyMgr::findProperty(const std::string & name)
    {
        PropertyMap::iterator it = m_properties.find(name);
        if(it != m_properties.end())
        {
            return it->second;
        }
        return  NULL;
    }
    
    bool PropertyMgr::registerProperty(rapidjson::Value & value)
    {
        PropertyNode *pNode = new PropertyNode();
        if(!pNode->load(value))
        {
            delete pNode;
            return false;
        }
        
        m_properties[pNode->getName()] = pNode;
        return true;
    }
    
    const std::string & PropertyMgr::cppNameToUIName(const std::string & className)
    {
        StringMap::iterator it = m_class2uiName.find(className);
        if(it != m_class2uiName.end())
        {
            return it->second;
        }
        
        return empty_string;
    }
    
    void PropertyMgr::registerUIName(const std::string & className, const std::string & uiName)
    {
        m_class2uiName[className] = uiName;
    }
    
} // end namespace Editor