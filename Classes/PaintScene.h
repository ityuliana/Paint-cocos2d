#ifndef __PAINT_SCENE_H__
#define __PAINT_SCENE_H__

#include "cocos2d.h"

const int LINE = 0;
const int CIRCLE = 1;
const int RECTANGLE = 2;
const int TRIANGLE = 3;
const int HEART = 4;

class PaintScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(PaintScene);

private:
    cocos2d::DrawNode* drawNode;
    cocos2d::DrawNode* previewNode;

    cocos2d::Vec2 startPoint;
    cocos2d::Color4F Color;
    int Figure;
    bool isDrawing;
    bool isFilled;

    void createUI();
    void createFigureButton(const std::string& name, int type, cocos2d::Vec2 pos);
    void createColorButton(cocos2d::Color4F color, cocos2d::Vec2 pos);
    void renderShapeTo(cocos2d::DrawNode* target, cocos2d::Vec2 center, cocos2d::Vec2 RadiusPoint);
    void createClearButton(const std::string& name, cocos2d::Vec2 pos);
    void createFillButton(const std::string& name, cocos2d::Vec2 pos);
};

#endif