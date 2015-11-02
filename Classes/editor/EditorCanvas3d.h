#ifndef CANVAS3D_H
#define CANVAS3D_H

#include "EditorCanvas.h"
#include <base/CCRefPtr.h>

NS_CC_BEGIN
class Camera;
class DrawNode3D;
NS_CC_END

namespace Editor
{

    class Canvas3D : public Canvas
    {
        Q_OBJECT
    public:
        Canvas3D(QObject *parent, GLWidget *view);

    public slots:
        virtual void onRootSet(cocos2d::Node *root) override;
        virtual void onPopertyChange(PropertyParam &param) override;
        virtual void onMouseEvent(QMouseEvent *event) override;
        virtual void onKeyEvent(QKeyEvent *event) override;
        virtual void onResize(float width, float height) override;

    private:
        virtual void drawSelectedRect() override;

        void doCameraRotation(const cocos2d::Vec2 &newPt, const cocos2d::Vec2 &oldPt);

        cocos2d::RefPtr<cocos2d::DrawNode3D>     drawNode_;
        cocos2d::RefPtr<cocos2d::Camera>         camera_;

        cocos2d::Point      lastMousePosition_;

        float               cameraMoveSpeed_;
    };

} // end namespace Editor

#endif // CANVAS3D_H
