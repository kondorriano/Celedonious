#include "Player.hpp"
#include "DeferredContainer.hpp"

Player::Player() : cam(nullptr), pos(0.0f), renderer(nullptr) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	model.program = Programs.get("deferredModel");
	model.mesh = Meshes.get("quad");
	cam = new Camera("playerCam",vec3f(0,0,20));
	cam->projection = glm::perspective(60.0f, float(Environment::getScreen()->getWidth())/float(Environment::getScreen()->getHeight()), 0.1f, 10000.0f);
	cam->addTo(this);
	rueda = new Physics::CircleCollider();
	rueda->setDType(Physics::Collider::Dynamic);
	rueda->setPosition(vec2f(0.0f));
	rueda->setDensity(1.0f);
	rueda->setFriction(10.0f);
	rueda->setRadius(0.5f);
	rueda->setRestitution(0.4f);
	this->addCollider(rueda);
	Physics::CircleCollider* rueda2 = new Physics::CircleCollider();
	rueda2->setDType(Physics::Collider::Dynamic);
	rueda2->setPosition(vec2f(0.0f));
	rueda2->setDensity(1.0f);
	rueda2->setFriction(10.0f);
	rueda2->setRadius(0.1f);
	rueda2->setRestitution(0.0f);
	rueda2->setFixedRotation(true);
	this->addCollider(rueda2);
	Physics::PolygonCollider* box = new Physics::PolygonCollider();
	box->setDType(Physics::Collider::Dynamic);
	box->setPosition(vec2f(0.0f,1.6f));
	box->setDensity(0.1f);
	box->setFriction(10.0f);
	box->setRestitution(0.4f);
	box->setAsBox(0.4f,1.0f);
	this->addCollider(box);
	Physics::RevoluteJointDef def;
	Physics::RevoluteJointDef def2;
	def.init(rueda2, box, vec2f(0.0f));
	def2.init(rueda, rueda2, vec2f(0.0f));
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
		float current = rueda->getAngularVelocity();
		if(current < 0.0f) rueda->setAngularVelocity(current*0.01f);
		if(rueda->getAngularVelocity() < 20.0f)  rueda->applyTorque(vel);
	}
	else if(Environment::getKeyboard()->isKeyHeld(Keyboard::Right)) {
		float current = rueda->getAngularVelocity();
		if(current > 0.0f) rueda->setAngularVelocity(current*0.01f);
		if(rueda->getAngularVelocity() > -20.0f) rueda->applyTorque(-vel);
	}
	else
		rueda->setAngularVelocity(rueda->getAngularVelocity()*0.6f);
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::UP)){
		if(rueda->getLinearVelocity().y < 5.0f) rueda->applyForceToCenterOfMass(vec2f(0.0f,80.0f));
	}
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::Down)){
		if(rueda->getLinearVelocity().y > -50.0f) rueda->applyForceToCenterOfMass(vec2f(0.0f,-80.0f));
	}
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::Space)) pos.z -= vel*deltaTime;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::LShift)) pos.z += vel*deltaTime;
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::R)) rueda->setRadius(rueda->getRadius()+0.1f);
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::F)) rueda->setRadius(rueda->getRadius()-0.1f);
	if(Environment::getKeyboard()->isKeyPressed(Keyboard::C)) rueda->deleteJoint(0);
	pos.x = rueda->getPosition().x;
	pos.y = rueda->getPosition().y;
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
