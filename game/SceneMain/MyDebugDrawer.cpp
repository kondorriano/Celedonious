#include "MyDebugDrawer.hpp"
#include "DeferredContainer.hpp"
#include "Manager.hpp"

MyDebugDrawer::MyDebugDrawer() : drawEnabled(true), gridEnabled(true), renderer(nullptr) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	this->SetFlags(b2Draw::e_shapeBit | b2Draw::e_aabbBit | b2Draw::e_centerOfMassBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_particleBit);
	std::vector<Vertex::Attribute> e = {Vertex::Attribute("a_position", Vertex::Attribute::Float, 2)};
	Vertex::Format f(e);
	std::vector<vec2f> coords;
	for(int i = 0; i < 360; i += 10) {coords.push_back(vec2f(cos(i*DEG_TO_RAD), sin(i*DEG_TO_RAD)));}
	circle = new Mesh(f,Mesh::DYNAMIC);
	circle->setVertexData(&coords[0], coords.size());
	poly = new Mesh(f,Mesh::DYNAMIC);
}

MyDebugDrawer::~MyDebugDrawer(){
	delete circle;
	delete poly;
}

void MyDebugDrawer::update(float deltaTime) {
	(void) deltaTime;
	 if(Keyboard::justPressed((Keyboard::E))) drawEnabled = !drawEnabled;
	 if(Keyboard::justPressed((Keyboard::R))) gridEnabled = !gridEnabled;
}

void MyDebugDrawer::draw() const {
	if(renderer->getMode() != DeferredContainer::Forward) return;
	if(drawEnabled) Physics::Engine::draw(this);
	if(gridEnabled) Physics::Engine::drawGrid(this, vec2f(-60.0f), vec2f(60.0f), 1.0f, vec4f(1.0f, 1.0f, 1.0f, 0.1f));
}

void MyDebugDrawer::drawParticles(vec2f* centers, float radius, vec4uc* colors, int count) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	poly->setPrimitiveType(Mesh::POINTS);
	poly->setVertexData(centers, count);
	Programs.get("debug").uniform("color")->set(vec4f(1.0f));
	Programs.get("debug").uniform("MVP")->set(cam->projection*cam->getView());
	poly->draw(Programs.get("debug"));
}

void MyDebugDrawer::drawPolygon(const vec2f* vertices, int vertexCount, const vec4f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	poly->setPrimitiveType(Mesh::LINE_LOOP);
	poly->setVertexData(vertices, vertexCount);
	Programs.get("debug").uniform("color")->set(color);
	Programs.get("debug").uniform("MVP")->set(cam->projection*cam->getView());
	poly->draw(Programs.get("debug"));
}

void MyDebugDrawer::drawSolidPolygon(const vec2f* vertices, int vertexCount, const vec4f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	poly->setPrimitiveType(Mesh::TRIANGLE_FAN);
	poly->setVertexData(vertices, vertexCount);
	Programs.get("debug").uniform("color")->set(color);
	Programs.get("debug").uniform("MVP")->set(cam->projection*cam->getView());
	poly->draw(Programs.get("debug"));
}

void MyDebugDrawer::drawCircle(const vec2f& center, float radius, const vec4f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	circle->setPrimitiveType(Mesh::LINE_LOOP);
	Programs.get("debug").uniform("color")->set(color);
	Programs.get("debug").uniform("MVP")->set(cam->projection*cam->getView()*glm::scale(glm::translate(mat4f(1.0f), vec3f(center, 0.0f)), vec3f(radius)));
	circle->draw(Programs.get("debug"));
}

void MyDebugDrawer::drawSolidCircle(const vec2f& center, float radius, const vec2f& axis, const vec4f& color) {
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	circle->setPrimitiveType(Mesh::TRIANGLE_FAN);
	Programs.get("debug").uniform("color")->set(color);
	Programs.get("debug").uniform("MVP")->set(cam->projection*cam->getView()*glm::scale(glm::translate(mat4f(1.0f), vec3f(center, 0.0f)), vec3f(radius)));
	circle->draw(Programs.get("debug"));
	drawSegment(center,center+radius*axis,color);
}

void MyDebugDrawer::drawSegment(const vec2f& p1, const vec2f& p2, const vec4f& color) {
	std::vector<vec2f> v;
	v.push_back(p1);
	v.push_back(p2);
	Camera* cam = (Camera*)Game::i()->getObjectByName("playerCam");
	poly->setPrimitiveType(Mesh::LINES);
	poly->setVertexData(&v[0], 2);
	Programs.get("debug").uniform("color")->set(color);
	Programs.get("debug").uniform("MVP")->set(cam->projection*cam->getView());
	poly->draw(Programs.get("debug"));
}

void MyDebugDrawer::drawTransform(const vec2f& position, const vec2f& xAxis, const vec2f& yAxis) {
	float k_axisScale = 0.5f;
	drawSegment(position,position + k_axisScale * xAxis, vec4f(1,0,0,0.5));
	drawSegment(position,position + k_axisScale * yAxis, vec4f(0,1,0,0.5));
}
