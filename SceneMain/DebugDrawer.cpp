#include "DebugDrawer.hpp"
#include "physics/PhysicsEngine.hpp"
#include "DeferredContainer.hpp"

DebugDrawer::DebugDrawer() : renderer(nullptr) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	this->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit | b2Draw::e_centerOfMassBit | b2Draw::e_jointBit | b2Draw::e_pairBit);
	std::vector<Vertex::Element> e = {Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 2)};
	Vertex::Format f(e);
	std::vector<vec2f> coords;
	for(int i = 0; i < 360; i += 10) {coords.push_back(vec2f(cos(i*DEG_TO_RAD), sin(i*DEG_TO_RAD)));}
	circle.mesh = Mesh::loadEmpty(f,Mesh::DYNAMIC);
	circle.mesh->setVertexData(&coords[0], coords.size());
	circle.program = Programs.get("debug");
	poly.mesh = Mesh::loadEmpty(f,Mesh::DYNAMIC);
	poly.program = Programs.get("debug");
}

DebugDrawer::~DebugDrawer(){
	delete circle.mesh;
	delete poly.mesh;
}

void DebugDrawer::draw() const {
	if(renderer->getMode() != DeferredContainer::Forward || !Environment::getKeyboard()->isKeyHeld(Keyboard::Space)) return;
	PhysicsEngine::draw(this);
	PhysicsEngine::drawGrid(this);
}

void DebugDrawer::drawPolygon(const vec2f* vertices, int vertexCount, const vec3f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	poly.mesh->setPrimitiveType(Mesh::LINE_LOOP);
	poly.mesh->setVertexData(vertices, vertexCount);
	poly.program->uniform("color")->set(vec3f(color.r,color.g,color.b));
	poly.program->uniform("MVP")->set(cam->projection*cam->getView());
	poly.draw();
}

void DebugDrawer::drawSolidPolygon(const vec2f* vertices, int vertexCount, const vec3f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	poly.mesh->setPrimitiveType(Mesh::TRIANGLE_FAN);
	poly.mesh->setVertexData(vertices, vertexCount);
	poly.program->uniform("color")->set(vec3f(color.r,color.g,color.b));
	poly.program->uniform("MVP")->set(cam->projection*cam->getView());
	poly.draw();
}

void DebugDrawer::drawCircle(const vec2f& center, float radius, const vec3f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	circle.mesh->setPrimitiveType(Mesh::LINE_LOOP);
	circle.program->uniform("color")->set(vec3f(color.r,color.g,color.b));
	circle.program->uniform("MVP")->set(cam->projection*cam->getView()*glm::scale(glm::translate(mat4f(1.0f), vec3f(center, 0.0f)), vec3f(radius)));
	circle.draw();
}

void DebugDrawer::drawSolidCircle(const vec2f& center, float radius, const vec2f& axis, const vec3f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	circle.mesh->setPrimitiveType(Mesh::TRIANGLE_FAN);
	circle.program->uniform("color")->set(vec3f(color.r,color.g,color.b));
	circle.program->uniform("MVP")->set(cam->projection*cam->getView()*glm::scale(glm::translate(mat4f(1.0f), vec3f(center, 0.0f)), vec3f(radius)));
	circle.draw();
	drawSegment(center,center+radius*axis,color);
}

void DebugDrawer::drawSegment(const vec2f& p1, const vec2f& p2, const vec3f& color) {
	std::vector<vec2f> v;
	v.push_back(p1);
	v.push_back(p2);
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	poly.mesh->setPrimitiveType(Mesh::LINES);
	poly.mesh->setVertexData(&v[0], 2);
	poly.program->uniform("color")->set(vec3f(color.r,color.g,color.b));
	poly.program->uniform("MVP")->set(cam->projection*cam->getView());
	poly.draw();
}

void DebugDrawer::drawTransform(const vec2f& position, const vec2f& xAxis, const vec2f& yAxis) {
	float k_axisScale = 0.5f;
	drawSegment(position,position + k_axisScale * xAxis, vec3f(1,0,0));
	drawSegment(position,position + k_axisScale * yAxis, vec3f(0,1,0));
}