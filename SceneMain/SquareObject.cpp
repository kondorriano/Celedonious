#include "SquareObject.hpp"
#include "DeferredContainer.hpp"
#include "physics/PolygonCollider.hpp"

SquareObject::SquareObject() {
	quad.mesh = Meshes.get("quad");
	quad.program = Programs.get("deferredModel");
	renderer = (DeferredContainer*)getGame()->getObjectByName("deferred");
	PolygonCollider* p = new PolygonCollider();
	p->setAsBox(1.0f,1.0f);
	p->setPosition(vec2f(-1.1f,0.0f));
	setCollider(p);
}

SquareObject::~SquareObject() {
}

void SquareObject::update(float deltaTime) {
	(void) deltaTime;
	transform = glm::translate(mat4f(1.0f), vec3f(getCollider()->getPosition(), 0.0f));
	transform = glm::rotate(transform, glm::degrees(getCollider()->getRotation()), vec3f(0, 0, 1));
}

void SquareObject::draw() const {
	if(renderer->getMode() != DeferredContainer::Deferred) return;
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	quad.program->uniform("MVP")->set(cam->projection*cam->getView()*fullTransform);
	quad.program->uniform("M")->set(fullTransform);
	quad.program->uniform("V")->set(cam->getView());
	quad.program->uniform("ambient")->set(0.5f);
	quad.program->uniform("specular")->set(1.0f);
	quad.program->uniform("diffuseTex")->set(Textures2D.get("nullRed"));
	quad.draw();
}
