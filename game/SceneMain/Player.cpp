#include "Player.hpp"
#include "DeferredContainer.hpp"
#include "Manager.hpp"

Player::Player() : cam(nullptr), pos(0.0f), renderer(nullptr), dir(Right) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	model = Meshes.get("quad");
	cam = new Camera("playerCam",vec3f(0,0,20));
	cam->projection = glm::perspective(60.0f, float(Window::getInstance()->getSize().x)/float(Window::getInstance()->getSize().y), 0.1f, 10000.0f);
	cam->addTo(this);
	Physics::CircleCollider* rueda = new Physics::CircleCollider();
	rueda->setDType(Physics::Collider::Dynamic);
	rueda->setPosition(vec2f(0.0f));
	rueda->setFriction(800.0f);
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
	def.enableMotor = true;
	def.lowerAngle = -0.3f;
	def.upperAngle = 0.3f;
	def.motorSpeed = 0.0f;
	def.maxMotorTorque = 100.0f;
	new Physics::RevoluteJoint(def);
	new Physics::RevoluteJoint(def2);
}

Player::~Player() {
}

void Player::update(float deltaTime) {
	float vel = 3000.0f;
	Physics::CircleCollider* wheel = (Physics::CircleCollider*)getCollider(Wheel);
	Physics::RevoluteJoint* axisJoint = (Physics::RevoluteJoint*)getCollider(Body)->getJoint(0);
	if(Keyboard::pressed(Keyboard::Left)) {
		dir = Left;
		axisJoint->setLimits(-0.3f, 0.3);
		if(wheel->getAngularVelocity() < 20.0f) wheel->applyTorque(vel*deltaTime);
		axisJoint->setMotorSpeed(1.0f);
	}
	else if(Keyboard::pressed(Keyboard::Right)) {
		dir = Right;
		axisJoint->setLimits(-0.3f, 0.3);
		if(wheel->getAngularVelocity() > -20.0f) wheel->applyTorque(-vel*deltaTime);
		axisJoint->setMotorSpeed(-1.0f);
	}
	else {
		wheel->setAngularVelocity(wheel->getAngularVelocity()*0.6f);
		axisJoint->setLimits(axisJoint->getLowerLimit()*0.6, axisJoint->getUpperLimit()*0.6);
	}
	if(Keyboard::pressed(Keyboard::UP))
		if(wheel->getLinearVelocity().y < 5.0f) wheel->applyForceToCenterOfMass(vec2f(0.0f,80.0f));
	if(Keyboard::pressed(Keyboard::Down))
		if(wheel->getLinearVelocity().y > -50.0f) wheel->applyForceToCenterOfMass(vec2f(0.0f,-80.0f));
	if(Keyboard::pressed(Keyboard::Space)) pos.z -= 30.0f*deltaTime;
	if(Keyboard::pressed(Keyboard::LShift)) pos.z += 30.0f*deltaTime;
	if(Keyboard::pressed(Keyboard::P)) {
		vec2f shootPos = getCollider(Body)->getWorldPoint(vec2f(dir==Right?0.6f:-0.6f, 0.0f));
		Physics::ParticleSystem* psys = (Physics::ParticleSystem*)getGame()->getObjectByName("psys");
		for(int i = 0; i < 5; ++i) {
			Physics::ParticleDef pd;
			pd.flags = Physics::WaterParticle;
			pd.position = shootPos + vec2f(0.0f,float(rand()%100)/200.0f);
			pd.velocity = vec2f(dir==Right?100.0f:-100.0f,0.0f);
			pd.lifetime = 1000;
			psys->createParticle(pd);
		}
	}
	pos.x = wheel->getPosition().x;
	pos.y = wheel->getPosition().y;
	transform = glm::translate(mat4f(1.0f), pos);
}

void Player::draw() const {
	if(renderer->getMode() != DeferredContainer::Deferred) return;
	Programs.get("deferredModel")->uniform("MVP")->set(cam->projection*cam->getView()*glm::scale(fullTransform, vec3f(0.1f)));
	Programs.get("deferredModel")->uniform("M")->set(fullTransform);
	Programs.get("deferredModel")->uniform("V")->set(cam->getView());
	Programs.get("deferredModel")->uniform("ambient")->set(0.5f);
	Programs.get("deferredModel")->uniform("specular")->set(1.0f);
	Programs.get("deferredModel")->uniform("diffuseTex")->set(Textures2D.get("nullGreen"));
	model->draw(Programs.get("deferredModel"));
}
