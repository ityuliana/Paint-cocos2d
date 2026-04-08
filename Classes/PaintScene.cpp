#include "PaintScene.h"
//#include <cmath>

USING_NS_CC;

Scene* PaintScene::createScene() {
    return PaintScene::create();
}

bool PaintScene::init() {
    if (!Scene::init()) return false;

    Size size = Director::getInstance()->getVisibleSize();

    auto bg = LayerColor::create(Color4B::WHITE);
    this->addChild(bg, 0);
    drawNode = DrawNode::create();
    this->addChild(drawNode, 1);

    previewNode = DrawNode::create();
    this->addChild(previewNode, 100);

    Figure = LINE;
    Color = Color4F::BLACK;
    isDrawing = false;
    isFilled = false;

    createUI();

    auto listener = EventListenerMouse::create();
    listener->onMouseDown = [this](EventMouse* event) {
        if (event->getCursorY() > Director::getInstance()->getVisibleSize().height - 60) return;
        if (event->getCursorX() > Director::getInstance()->getVisibleSize().width - 80) return;

        startPoint = Vec2(event->getCursorX(), event->getCursorY());
        isDrawing = true;
        };

    listener->onMouseMove = [this](EventMouse* event) {
        if (!isDrawing) return;

        Vec2 current = Vec2(event->getCursorX(), event->getCursorY());

        previewNode->clear();
        renderShapeTo(previewNode, startPoint, current);
        };

    listener->onMouseUp = [this](EventMouse* event) {
        if (!isDrawing) return;

        Vec2 endPoint = Vec2(event->getCursorX(), event->getCursorY());

        previewNode->clear();
        renderShapeTo(drawNode, startPoint, endPoint);

        isDrawing = false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void PaintScene::createUI() {
    Size size = Director::getInstance()->getVisibleSize();

    auto uiLine = DrawNode::create();
    uiLine->drawLine(Vec2(0, size.height - 55), Vec2(size.width, size.height - 55), Color4F::BLACK);
    uiLine->drawLine(Vec2(size.width - 80, 0), Vec2(size.width - 80, size.height - 55), Color4F::BLACK);
    this->addChild(uiLine, 5);

    createFigureButton("Line", LINE, Vec2(60, size.height - 30));
    createFigureButton("Circle", CIRCLE, Vec2(150, size.height - 30));
    createFigureButton("Rect", RECTANGLE, Vec2(240, size.height - 30));
    createFigureButton("Triangle",TRIANGLE, Vec2(330, size.height - 30));
    createFigureButton("Heart", HEART, Vec2(420, size.height - 30));
    createFillButton("Fill", Vec2(1080, size.height - 30));
    createClearButton("Clear", Vec2(size.width - 50, size.height - 30));

    float colorX = size.width - 40;
    createColorButton(Color4F::BLACK, Vec2(colorX, size.height - 100));
    createColorButton(Color4F::RED, Vec2(colorX, size.height - 150));
    createColorButton(Color4F::ORANGE, Vec2(colorX, size.height - 200));
    createColorButton(Color4F::YELLOW, Vec2(colorX, size.height - 250));
    createColorButton(Color4F::GREEN, Vec2(colorX, size.height - 300));
    createColorButton(Color4F::BLUE, Vec2(colorX, size.height - 350));
    createColorButton(Color4F::MAGENTA, Vec2(colorX, size.height - 400));
}

void PaintScene::createFigureButton(const std::string& name, int type, Vec2 pos) {
    auto label = Label::createWithSystemFont(name, "Segoe UI", 25);
    label->setPosition(pos);
    label->setColor(Color3B::BLACK);
    this->addChild(label, 10);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this, label, type](Touch* touch, Event*) {
        if (label->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            this->Figure = type;
            label->runAction(Sequence::create(ScaleTo::create(0.05f, 1.2f), ScaleTo::create(0.05f, 1.0f), nullptr));
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, label);
}

void PaintScene::createColorButton(Color4F color, Vec2 pos) {
    auto node = DrawNode::create();
    node->drawSolidCircle(Vec2::ZERO, 18, 0, 30, color);
    node->drawCircle(Vec2::ZERO, 18, 0, 30, false, Color4F::GRAY);
    node->setPosition(pos);
    this->addChild(node, 10);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this, node, color](Touch* touch, Event*) {
        Vec2 localP = node->convertToNodeSpace(touch->getLocation());
        if (localP.length() < 18) { 
            this->Color = color;
            node->runAction(Sequence::create(ScaleTo::create(0.05f, 1.2f), ScaleTo::create(0.05f, 1.0f), nullptr));
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, node);
}

void PaintScene::renderShapeTo(DrawNode* target, Vec2 center, Vec2 RadiusPoint) {
    float dx = RadiusPoint.x - center.x;
    float dy = RadiusPoint.y - center.y;
    float radius = sqrt(dx * dx + dy * dy);
    switch (Figure) {

    case LINE:
        target->drawLine(center, RadiusPoint, Color);
        break;

    case CIRCLE:
        if (isFilled)
            target->drawSolidCircle(center, radius, 0, 64, Color);
        else
            target->drawCircle(center, radius, 0, 64, false, Color);
        break;

    case RECTANGLE:
        if (isFilled)
            target->drawSolidRect(center, RadiusPoint, Color);
        else
            target->drawRect(center, RadiusPoint, Color);
        break;

    case TRIANGLE: {
        Vec2 points[3];
        points[0] = center + Vec2(0, radius);
        points[1] = center + Vec2(-radius * 0.866f, -radius / 2); //√3/2 
        points[2] = center + Vec2(radius * 0.866f, -radius / 2);
      
        if (isFilled)
            target->drawSolidPoly(points, 3, Color);
        else
            target->drawPoly(points, 3, true, Color);
        break;
    }

    case HEART: {
        float size = radius;
        Vec2 points[100];

        for (int i = 0; i < 100; i++) {
            float t = i * 2 * M_PI / 100;

            float x = 16 * pow(sin(t), 3);
            float y = 13 * cos(t)
                - 5 * cos(2 * t)
                - 2 * cos(3 * t)
                - cos(4 * t);

            points[i] = center + Vec2(x, y) * (size / 20);
        }
        if (isFilled)
            target->drawSolidPoly(points, 100, Color);
        else
            target->drawPoly(points, 100, true, Color);
        break;
    }
    }
}

void PaintScene::createClearButton(const std::string& name, Vec2 pos) {
    auto label = Label::createWithSystemFont(name, "Segoe UI", 25);
    label->setPosition(pos);
    label->setColor(Color3B::BLACK);
    this->addChild(label, 10);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this, label](Touch* touch, Event*) {
        if (label->getBoundingBox().containsPoint(this->convertToNodeSpace(touch->getLocation()))) {
            drawNode->clear();
            label->runAction(Sequence::create(ScaleTo::create(0.05f, 1.2f), ScaleTo::create(0.05f, 1.0f), nullptr));
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, label);
}

void PaintScene::createFillButton(const std::string& name, Vec2 pos)
{
    auto label = Label::createWithSystemFont(name, "Segoe UI", 25);
    label->setPosition(pos);
    label->setColor(Color3B::BLACK);
    this->addChild(label, 10);

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = [this, label](Touch* touch, Event*)
        {
            if (label->getBoundingBox().containsPoint(
                this->convertToNodeSpace(touch->getLocation())))
            {
                isFilled = !isFilled; // переключення режиму

                label->runAction(
                    Sequence::create(ScaleTo::create(0.05f, 1.2f), ScaleTo::create(0.05f, 1.0f),nullptr));

                return true;
            }
            return false;
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, label);
}