#include "Player.hpp"
#include "DeferredContainer.hpp"

Player::Player() : cam(nullptr), pos(0.0f), renderer(nullptr) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	model.program = Programs.get("deferredModel");
	model.mesh = Meshes.get("quad");
	cam = new Camera("playerCam",vec3f(0,0,10));
	cam->projection = glm::perspective(60.0f, float(Environment::getScreen()->getWidth())/float(Environment::getScreen()->getHeight()), 0.1f, 100.0f);
	cam->addTo(this);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
	float vel = 10.0f;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::W)) pos.y += vel*deltaTime;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::A)) pos.x -= vel*deltaTime;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::S)) pos.y -= vel*deltaTime;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::D)) pos.x += vel*deltaTime;
	transform = glm::translate(mat4f(1.0f),pos);
}

void Player::draw() const {
	if(renderer->getMode() != DeferredContainer::Deferred) return;
	model.program->uniform("MVP")->set(cam->projection*cam->getView()*fullTransform);
	model.program->uniform("M")->set(fullTransform);
	model.program->uniform("V")->set(cam->getView());
	model.program->uniform("ambient")->set(0.5f);
	model.program->uniform("specular")->set(1.0f);
	model.program->uniform("diffuseTex")->set(Textures2D.get("nullGreen"));
	model.draw();
}
