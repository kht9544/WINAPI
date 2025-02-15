#include "pch.h"
#include "PaintScene.h"

PaintScene::PaintScene()
{
    _rect = make_shared<RectCollider>(Vector2(0.0f,0.0f), Vector2(50.0f,50.0f));

    _circle= make_shared<CircleCollider>(Vector2(50.0f,50.0f), 15);
}

PaintScene::~PaintScene()
{
}

void PaintScene::Update()
{
    _rect->_center = _rect->_center + Vector2(0.5f,0.5f);

    _circle->_center = LERP(_circle->_center, mousePos, 0.1f);

    _rect->Update();
    _circle->Update();
}

void PaintScene::Render(HDC hdc)
{

    _rect->Render(hdc);


    _circle->Render(hdc);
}
