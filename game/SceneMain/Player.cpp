#include "Player.hpp"
#include "DeferredContainer.hpp"
#include "Manager.hpp"

Player::Player() : cam(nullptr), pos(0.0f), renderer(nullptr), dir(Right) {
	renderer = (DeferredContainer*) getGame()->getObjectByName("deferred");
	model = &Meshes.get("quad");
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

	Physics::PolygonCollider* head = new Physics::PolygonCollider();
	head->setDType(Physics::Collider::Dynamic);
	head->setPosition(vec2f(0.0f,2.6f));
	head->setDensity(0.00001f);
	head->setAsBox(0.8f,0.3f);
	this->addCollider(head);

	Physics::PolygonCollider* body = new Physics::PolygonCollider();
	body->setDType(Physics::Collider::Dynamic);
	body->setPosition(vec2f(0.0f,1.5f));
	body->setDensity(0.1f);
	body->setFriction(10.0f);
	body->setRestitution(0.4f);
	body->setAsBox(0.3f,0.65f);
	this->addCollider(body);

	Physics::PolygonCollider* jetpack = new Physics::PolygonCollider();
	jetpack->setDType(Physics::Collider::Dynamic);
	jetpack->setPosition(vec2f(0.0f,1.6f));
	jetpack->setDensity(0.00001f);
	jetpack->setAsBox(0.1f,0.35f);
	this->addCollider(jetpack);

	Physics::CircleCollider* axis = new Physics::CircleCollider();
	axis->setDType(Physics::Collider::Dynamic);
	axis->setPosition(vec2f(0.0f));
	axis->setFriction(10.0f);
	axis->setRadius(0.1f);
	axis->setFixedRotation(true);
	this->addCollider(axis);

	Physics::CircleCollider* sensor = new Physics::CircleCollider();
	sensor->setSensor(true);
	sensor->setDType(Physics::Collider::Dynamic);
	sensor->setPosition(vec2f(0.0f,-0.5f));
	sensor->setRadius(0.3f);
	sensor->setDensity(0.00001f);
	this->addCollider(sensor);

	Physics::PolygonCollider* eyes = new Physics::PolygonCollider();
	eyes->setDType(Physics::Collider::Dynamic);
	eyes->setPosition(vec2f(0.5f,2.6f));
	eyes->setDensity(0.00001f);
	eyes->setAsBox(0.2f,0.1f);
	this->addCollider(eyes);

	Physics::RevoluteJointDef def;
	Physics::RevoluteJointDef def2;
	Physics::WeldJointDef def3;
	Physics::WeldJointDef def4;
	Physics::WeldJointDef def5;
	Physics::WeldJointDef def6;



	def.init(axis, body, vec2f(0.0f));
	def2.init(rueda, axis, vec2f(0.0f));
	def3.init(sensor,axis,vec2f(0.0f));
	def4.init(jetpack,body,vec2f(0.0f));
	def5.init(head,body,vec2f(0.0f));
	def6.init(eyes,head,vec2f(0.0f));



	def.enableLimit = true;
	def.enableMotor = true;
	def.lowerAngle = -0.3f;
	def.upperAngle = 0.3f;
	def.motorSpeed = 0.0f;
	def.maxMotorTorque = 100.0f;
	new Physics::RevoluteJoint(def);
	new Physics::RevoluteJoint(def2);
	new Physics::WeldJoint(def3);
	new Physics::WeldJoint(def4);
	new Physics::WeldJoint(def5);
	new Physics::WeldJoint(def6);




}

Player::~Player() {
}

void Player::update(float deltaTime) {
	if(Keyboard::pressed(Keyboard::Space)) pos.z -= 30.0f*deltaTime;
	if(Keyboard::pressed(Keyboard::LShift)) pos.z += 30.0f*deltaTime;
	movement(deltaTime);
	shoot(deltaTime);
	waterRefill(deltaTime);

}

void Player::movement(float deltaTime)
{
	Physics::CircleCollider* wheel = (Physics::CircleCollider*)getCollider(Wheel);
	Physics::RevoluteJoint* axisJoint = (Physics::RevoluteJoint*)getCollider(Body)->getJoint(0);
	Physics::PolygonCollider* jetpack = (Physics::PolygonCollider*)getCollider(JetPack);
	Physics::PolygonCollider* body = (Physics::PolygonCollider*)getCollider(Body);


	float vel = 3000.0f;
	float axisX = Gamepad::axis(0,Gamepad::AxisLeftX);
	if(Keyboard::pressed(Keyboard::Left)) axisX = -1.0f;
	if(Keyboard::pressed(Keyboard::Right)) axisX = 1.0f;

	//Movimiento Horizontal
	//Floor
	if(colliding) {
		if(axisX > 0.25f) {
			//Move Right
			dir = Right;
			axisJoint->setLimits(glm::min(-0.3f*glm::abs(axisX), axisJoint->getLowerLimit()*0.95f) ,
								 glm::max(0.3f*glm::abs(axisX), axisJoint->getUpperLimit()*0.95f));
			if(wheel->getAngularVelocity() > -20.0f) wheel->applyTorque(-vel*deltaTime*glm::abs(axisX));
			axisJoint->setMotorSpeed(-1.0f);
		} else if(axisX < -0.25f) {
			//Move Left
			dir = Left;
			axisJoint->setLimits(glm::min(-0.3f*glm::abs(axisX), axisJoint->getLowerLimit()*0.95f) ,
								 glm::max(0.3f*glm::abs(axisX), axisJoint->getUpperLimit()*0.95f));
			if(wheel->getAngularVelocity() < 20.0f) wheel->applyTorque(vel*deltaTime*glm::abs(axisX));
			axisJoint->setMotorSpeed(1.0f);
		} else {
			wheel->setAngularVelocity(wheel->getAngularVelocity()*0.0f);
			axisJoint->setLimits(axisJoint->getLowerLimit()*0.95f, axisJoint->getUpperLimit()*0.95f);

		}
	//Air
	} else {
		vec2f vel = wheel->getLinearVelocity();
		if(axisX > 0.25f) {
			//Move Right
			axisJoint->setLimits(glm::min(-0.3f*glm::abs(axisX), axisJoint->getLowerLimit()*0.95f) ,
								 glm::max(0.3f*glm::abs(axisX), axisJoint->getUpperLimit()*0.95f));
			axisJoint->setMotorSpeed(-1.0f);

			if(vel.x < 10.0f) vel.x += 20*deltaTime*glm::abs(axisX);
		} else if(axisX < -0.25f) {
			//Move Left
			axisJoint->setLimits(glm::min(-0.3f*glm::abs(axisX), axisJoint->getLowerLimit()*0.95f) ,
								 glm::max(0.3f*glm::abs(axisX), axisJoint->getUpperLimit()*0.95f));
			axisJoint->setMotorSpeed(1.0f);

			if(vel.x > -10.0f) vel.x += -20*deltaTime*glm::abs(axisX);
		} else axisJoint->setLimits(axisJoint->getLowerLimit()*0.95f, axisJoint->getUpperLimit()*0.95f);

		wheel->setLinearVelocity(vel);
		wheel->setAngularVelocity(wheel->getAngularVelocity()*0.0f);
		//axisJoint->setLimits(axisJoint->getLowerLimit()*0.95f, axisJoint->getUpperLimit()*0.95f);

	}

	//JetPack Position
	if(dir == Right) {
		jetpack->setPosition(vec2f(body->getPosition().x-0.7f,wheel->getPosition().y+1.6f));
		jetpack->setRotation(body->getRotation()-0.2f);

	} else if(dir == Left) {
		jetpack->setPosition(vec2f(body->getPosition().x+0.7f,wheel->getPosition().y+1.6f));
		jetpack->setRotation(body->getRotation()+0.2f);

	}


	usingWater = false;
	bool jumping = false;
	//Jump
	boostFrameCoolDown = glm::max(0.0f,boostFrameCoolDown-deltaTime);
	if(Gamepad::justPressed(0,Gamepad::ButtonA) || Keyboard::justPressed(Keyboard::UP)) {
		if(colliding && boostFrameCoolDown == 0) {
			if(waterCounter >= jumpImpulseCost) {
				wheel->applyLinearImpulse(vec2f(0.0f,jumpImpulse),vec2f(0.0f));
				for(int i = 0; i < 20; ++i) createParticle(vec2f(dir,1.0f)+vec2f(this->pos), vec2f(dir*0.5f,-1.0f));

				waterCounter = glm::max(waterCounter-jumpImpulseCost,0.0f);
				usingWater = true;
				jumping = true;
				boostFrameCoolDown = 0.1f;
			}

		} else {
			/*
			if(waterCounter >= jumpImpulseCost*2) {
				wheel->applyLinearImpulse(vec2f(0.0f,jumpImpulse),vec2f(0.0f));
				waterCounter = glm::max(waterCounter-jumpImpulseCost*2,0.0f);
				usingWater = true;
			}*/
		}
	}

	if(Gamepad::pressed(0,Gamepad::ButtonA)|| Keyboard::pressed(Keyboard::UP)) {
		if(!colliding) {
			if(waterCounter != 0) {
				wheel->applyForceToCenterOfMass(vec2f(0.0f,jumpForce*deltaTime));

				createParticle(vec2f(dir,1.0f)+vec2f(this->pos), vec2f(dir*0.5f,-1.0f));

				waterCounter = glm::max(waterCounter-deltaTime*jumpForceCost,0.0f);
				usingWater = true;
				jumping = true;
			}
		}
	}


	//Fall
	if(!jumping) {
		vec2f vel = wheel->getLinearVelocity();
		vel.y -= 9.8f*deltaTime;
		wheel->setLinearVelocity(vel);

	}

	Log::message() << "water: " << waterCounter << " vel: " << wheel->getLinearVelocity().x<< Log::Flush;

	pos.x = wheel->getPosition().x;
	pos.y = wheel->getPosition().y;
	transform = glm::translate(mat4f(1.0f), pos);
}

void Player::shoot(float deltaTime)
{
	float axisX = Gamepad::axis(0,Gamepad::AxisRightX);
	float axisY = Gamepad::axis(0,Gamepad::AxisRightY);

	if(Keyboard::pressed(Keyboard::A)) axisX = -1.0f;
	if(Keyboard::pressed(Keyboard::D)) axisX = 1.0f;
	if(Keyboard::pressed(Keyboard::W)) axisY = -1.0f;
	if(Keyboard::pressed(Keyboard::S)) axisY = 1.0f;
	axisY = -axisY;

	if(!(glm::abs(axisX) > 0.25f || glm::abs(axisY) > 0.25f)) return;
	vec2f direction = vec2f(0.0f);
	if(glm::abs(axisX) > 0.25f) direction.x = axisX;
	if(glm::abs(axisY) > 0.25f) direction.y = axisY;
	direction = glm::normalize(direction);

	if(rand()%2)createParticle(vec2f(0.0f,1.0f)+vec2f(this->pos)+direction*2.0f, direction);

	waterCounter = glm::max(waterCounter-deltaTime,0.0f);
	usingWater = true;

}

void Player::waterRefill(float deltaTime)
{
	if(colliding && !usingWater) waterCounter = glm::min(waterCounter+deltaTime*waterRefillVelocity,waterTime);
}

void Player::createParticle(vec2f position, vec2f direction)
{
	Physics::ParticleSystem* psys = (Physics::ParticleSystem*)getGame()->getObjectByName("psys");
	Physics::ParticleDef pd;
	pd.flags = Physics::WaterParticle;
	pd.position = position + vec2f(float(rand()%100)/200.0f, float(rand()%100)/200.0f) + vec2f(-0.25f);
	pd.velocity = glm::normalize(direction)*100.0f;
	pd.lifetime = 10;
	psys->createParticle(pd);
}

void Player::draw() const {
	if(renderer->getMode() != DeferredContainer::Deferred) return;
	Programs.get("deferredModel").uniform("MVP")->set(cam->projection*cam->getView()*glm::scale(fullTransform, vec3f(0.1f)));
	Programs.get("deferredModel").uniform("M")->set(fullTransform);
	Programs.get("deferredModel").uniform("V")->set(cam->getView());
	Programs.get("deferredModel").uniform("ambient")->set(0.5f);
	Programs.get("deferredModel").uniform("specular")->set(1.0f);
	Programs.get("deferredModel").uniform("diffuseTex")->set(Textures2D.get("nullGreen"));
	model->draw(Programs.get("deferredModel"));
}

void Player::onCollisionEnter(int colliderIndex, const Physics::BodyContact* contact)
{
	if(colliderIndex == Sensor) {
		colliding = true;
		Log::message() << "Collide: " << colliding << Log::Flush;
	}

}

void Player::onCollisionExit(int colliderIndex, const Physics::BodyContact* contact)
{
	if(colliderIndex == Sensor) {
		colliding = false;
		Log::message() << "NotCollide: " << colliding << Log::Flush;
	}


}
