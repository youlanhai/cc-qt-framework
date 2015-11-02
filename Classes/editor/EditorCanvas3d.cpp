#include "EditorCanvas3D.h"

#include "DrawNode3D.h"
#include "Editor.h"

#include <2d/CCCamera.h>
#include <base/CCDirector.h>

#include <QMouseEvent>
#include <QKeyEvent>


USING_NS_CC;

namespace Editor
{

    Canvas3D::Canvas3D(QObject *parent, GLWidget *view)
        : Canvas(parent, view)
        , cameraMoveSpeed_(100.0f)
    {
        auto director = Director::getInstance();

        drawNode_ = DrawNode3D::create();
        Editor::instance()->getScene()->addChild(drawNode_);

        camera_ = Editor::instance()->getScene()->getDefaultCamera();
        onResize(director->getWinSize().width, director->getWinSize().height);

        camera_->setPosition3D(Vec3(0.0f, 0.0f, 100.0f));
        camera_->lookAt(Vec3::ZERO);
    }

    void Canvas3D::onRootSet(cocos2d::Node *root)
    {
        Canvas::onRootSet(root);
    }

    void Canvas3D::onPopertyChange(PropertyParam &param)
    {

    }

    void Canvas3D::onMouseEvent(QMouseEvent *event)
    {
        if(event->type() == QEvent::MouseButtonPress)
        {
            cocos2d::Point pt(event->x(), event->y());
            lastMousePosition_ = cocos2d::Director::getInstance()->convertToUI(pt);
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {

        }
        else if(event->type() == QEvent::MouseMove)
        {
            if(event->buttons() & Qt::RightButton)
            {
                cocos2d::Point pt(event->x(), event->y());
                pt = cocos2d::Director::getInstance()->convertToUI(pt);
                doCameraRotation(pt, lastMousePosition_);
                lastMousePosition_ = pt;
            }
        }
        else if(event->type() == QEvent::MouseTrackingChange)
        {

        }
    }

    void Canvas3D::onKeyEvent(QKeyEvent *event)
    {

    }

    void Canvas3D::onResize(float width, float height)
    {
        float aspect = width / height;
        camera_->initPerspective(60, aspect, 1.0f, 1000.0f);

        camera_->setViewport(cocos2d::experimental::Viewport(0, 0, width, height));
    }

    void Canvas3D::drawSelectedRect()
    {

    }

    void Canvas3D::doCameraRotation(const cocos2d::Vec2 &newPt, const cocos2d::Vec2 &oldPt)
    {
        Vec2 delta = newPt - oldPt;

        Size winSize = Director::getInstance()->getWinSize();

        float dx = delta.x / winSize.width;
        float dy = delta.y / winSize.height;

        Vec3 rotation = camera_->getRotation3D();
        rotation.y -= dx * 180.0f;
        rotation.x += dy * 180.0f;
        camera_->setRotation3D(rotation);
    }
}
