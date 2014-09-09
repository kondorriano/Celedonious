#include "Player.hpp"
#include "DeferredContainer.hpp"

Player::Player() : cam(nullptr), pos(0.0f), renderer(nullptr) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	model.program = Programs.get("deferredModel");
	model.mesh = Meshes.get("quad");
	cam = new Camera("playerCam",vec3f(0,0,20));
	cam->projection = glm::perspective(60.0f, float(Environment::getScreen()->getWidth())/float(Environment::getScreen()->getHeight()), 0.1f, 10000.0f);
	cam->addTo(this);
	Physics::CircleCollider* rueda = new Physics::CircleCollider();
	rueda->setDType(Physics::Collider::Dynamic);
	rueda->setPosition(vec2f(0.0f));
	rueda->setFriction(10.0f);
	rueda->setRadius(0.5f);
	rueda->setRestitution(0.4f);
	this->addCollider(rueda);
	Physics::PolygonCollider* body = new Physics::PolygonCollider();
	body->setDType(Physics::Collider::Dynamic);
	body->setPosition(vec2f(0.0f,1.6f));
	body->setDensity(0.1f);
	body->setFriction(10.0f);
	body->setRestitution(0.4f);
	body->setAsBox(0.4f,1.0f);
	this->addCollider(body);
	Physics::CircleCollider* axis = new Physics::CircleCollider();
	axis->setDType(Physics::Collider::Dynamic);
	axis->setPosition(vec2f(0.0f));
	axis->setFriction(10.0f);
	axis->setRadius(0.1f);
	axis->setFixedRotation(true);
	this->addCollider(axis);
	Physics::RevoluteJointDef def;
	Physics::RevoluteJointDef def2;
	def.init(axis, body, vec2f(0.0f));
	def2.init(rueda, axis, vec2f(0.0f));
	def.enableLimit = true;
	def.lowerAngle = -0.3f;
	def.upperAngle = 0.3f;
	new Physics::RevoluteJoint(def);
	new Physics::RevoluteJoint(def2);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
	float vel = 30.0f;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::Left)) {
		float current = getCollider(Wheel)->getAngularVelocity();
		if(current < 0.0f) getCollider(Wheel)->setAngularVelocity(current*0.01f);
		if(getCollider(Wheel)->getAngularVelocity() < 20.0f) getCollider(Wheel)->applyTorque(vel);
	}
	else if(Environment::getKeyboard()->isKeyHeld(Keyboard::Right)) {
		float current = getCollider(Wheel)->getAngularVelocity();
		if(current > 0.0f) getCollider(Wheel)->setAngularVelocity(current*0.01f);
		if(getCollider(Wheel)->getAngularVelocity() > -20.0f) getCollider(Wheel)->applyTorque(-vel);
	}
	else
		getCollider(Wheel)->setAngularVelocity(getCollider(Wheel)->getAngularVelocity()*0.6f);
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::UP)){
		if(getCollider(Wheel)->getLinearVelocity().y < 5.0f) getCollider(Wheel)->applyForceToCenterOfMass(vec2f(0.0f,80.0f));
	}
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::Down)){
		if(getCollider(Wheel)->getLinearVelocity().y > -50.0f) getCollider(Wheel)->applyForceToCenterOfMass(vec2f(0.0f,-80.0f));
	}
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::Space)) pos.z -= vel*deltaTime;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::LShift)) pos.z += vel*deltaTime;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::R)) getCollider(Wheel)->setRadius(getCollider(Wheel)->getRadius()+0.1f);
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::F)) getCollider(Wheel)->setRadius(getCollider(Wheel)->getRadius()-0.1f);
	if(Environment::getKeyboard()->isKeyPressed(Keyboard::C)) getCollider(Wheel)->deleteJoint(0);
	pos.x = getCollider(Wheel)->getPosition().x;
	pos.y = getCollider(Wheel)->getPosition().y;
	transform = glm::translate(mat4f(1.0f), pos);
}

void Player::draw() const {
	if(renderer->getMode() != DeferredContainer::Deferred) return;
	model.program->uniform("MVP")->set(cam->projection*cam->getView()*glm::scale(fullTransform, vec3f(0.1f)));
	model.program->uniform("M")->set(fullTransform);
	model.program->uniform("V")->set(cam->getView());
	model.program->uniform("ambient")->set(0.5f);
	model.program->uniform("specular")->set(1.0f);
	model.program->uniform("diffuseTex")->set(Textures2D.get("nullGreen"));
	model.draw();
}
