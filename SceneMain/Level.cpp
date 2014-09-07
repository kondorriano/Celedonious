#include "Level.hpp"
#include "DeferredContainer.hpp"

Level::Level() {
	quad.mesh = Meshes.get("quad");
	quad.program = Programs.get("deferredModel");
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
	Physics::PolygonCollider* p = new Physics::PolygonCollider();
	p->setAsBox(1.0f,1.0f);
	p->setDType(Physics::Collider::Static);
	p->setPosition(vec2f(0.0f));
	addCollider(p);
}

Level::~Level() {
}

void Level::update(float deltaTime) {
	(void) deltaTime;
}

void Level::draw() const {
	if(renderer->getMode() != DeferredContainer::Deferred) return;
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	quad.program->uniform("MVP")->set(cam->projection*cam->getView()*fullTransform);
	quad.program->uniform("M")->set(fullTransform);
	quad.program->uniform("V")->set(cam->getView());
	quad.program->uniform("ambient")->set(0.5f);
	quad.program->uniform("specular")->set(1.0f);
	quad.program->uniform("diffuseTex")->set(Textures2D.get("nullBlue"));
	quad.draw();
}
