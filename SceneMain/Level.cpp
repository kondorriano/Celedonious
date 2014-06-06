#include "Level.hpp"
#include "DeferredContainer.hpp"
#include "physics/PolygonCollider.hpp"

Level::Level() {
	quad.mesh = Meshes.get("quad");
	quad.program = Programs.get("deferredModel");
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
	PolygonCollider* p = new PolygonCollider();
	p->setAsBox(1.0f,1.0f);
	p->setDType(Collider::Static);
	p->setPosition(vec2f(0.0f));
	setCollider(p);
}

Level::~Level() {
}

void Level::update(float deltaTime) {
	(void) deltaTime;
	transform = glm::translate(mat4f(1.0f), vec3f(getCollider()->getPosition(), 0.0f));
	transform = glm::rotate(transform, getCollider()->getRotation(), vec3f(0, 0, 1));
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
