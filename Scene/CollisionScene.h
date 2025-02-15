#pragma once
class CollisionScene : public Scene
{
public:
	CollisionScene();
	~CollisionScene();

	void Update() override;
	void Render(HDC hdc) override;

private:
	shared_ptr<Collider> _circle1;
	shared_ptr<Collider> _moveCircle;
	shared_ptr<Collider> _rect1;
	shared_ptr<Collider> _moveRect;
};

