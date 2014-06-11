#include "SceneMain.hpp"
#include "DeferredContainer.hpp"
#include "BlurContainer.hpp"
#include "GenericBody.hpp"
#include "DeferredLight.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "DebugDrawer.hpp"
#include "physics/PhysicsEngine.hpp"
#include "physics/PolygonCollider.hpp"
#include "physics/CircleCollider.hpp"

SceneMain::SceneMain() : debugCounter(0.0f), fpsCount(0), pCount(0) {
	this->setName("SCENE");

	loadResources();
	srand(Environment::getClock());

	//GL stuff..:
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE); //enable backface culling
	glCullFace(GL_BACK);
	glPointSize(4.0f);

	PhysicsEngine::init();

	b2ParticleSystemDef def;
	def.radius = 0.15f;
	psys = PhysicsEngine::world->CreateParticleSystem(&def);

	BlurContainer* blur = new BlurContainer();
	blur->addTo(this);

	DeferredContainer* renderer = new DeferredContainer();
	renderer->addTo(blur);

	DebugDrawer* dbg = new DebugDrawer();
	dbg->addTo(renderer);

	Player* player = new Player();
	player->addTo(renderer);

	GenericBody* b = new GenericBody();
	PolygonCollider* p = new PolygonCollider();
	p->setAsBox(500.0f,1.0f);
	p->setDType(Collider::Static);
	p->setPosition(vec2f(0.0f, -10.0f));
	p->setFriction(100.0f);
	p->setRestitution(0.1f);
	b->set(p);
	b->addTo(renderer);

	b = new GenericBody();
	p = new PolygonCollider();
	p->setAsBox(1.0f,110.0f);
	p->setDType(Collider::Static);
	p->setPosition(vec2f(-10.0f,100.0f));
	b->set(p);
	b->addTo(renderer);

	b = new GenericBody();
	p = new PolygonCollider();
	p->setAsBox(1.0f,110.0f);
	p->setDType(Collider::Static);
	p->setPosition(vec2f(10.0f,100.0f));
	b->set(p);
	b->addTo(renderer);

	wall = b;

	DeferredLight* dl = new DeferredLight();
	dl->pos = vec3f(5.0f,5.0f,5.0f);
	dl->addTo(renderer);
}

SceneMain::~SceneMain() {
	Textures2D.clear();
	Meshes.clear();
	Programs.clear();
	PhysicsEngine::close();
}

void SceneMain::loadResources() {
	//meshes
	std::vector<Vertex::Element> elems = {
		Vertex::Element(Vertex::Attribute::Position, Vertex::Element::Float, 3)
	};
	std::vector<vec3f> data = {
		vec3f(1, -1, 0), vec3f(1, 1, 0), vec3f(-1, 1, 0),
		vec3f(-1, 1, 0), vec3f(-1, -1, 0), vec3f(1, -1, 0)
	};
	Mesh* quad = Mesh::loadEmpty(Vertex::Format(elems));
	quad->setVertexData(&data[0], 6);
	quad->setPrimitiveType(Mesh::TRIANGLES);
	Meshes.add("quad", quad);
	Meshes.add("monkey", Mesh::loadFromFile("data/meshes/monkey.obj"));

	//textures
	char pixels[4] = {char(200), char(20), char(20), char(255)};
	Textures2D.add("nullRed", Texture2D::createFromRaw(pixels, 1, 1));
	char pixels2[4] = {char(20), char(200), char(20), char(255)};
	Textures2D.add("nullGreen", Texture2D::createFromRaw(pixels2, 1, 1));
	char pixels3[4] = {char(20), char(20), char(200), char(255)};
	Textures2D.add("nullBlue", Texture2D::createFromRaw(pixels3, 1, 1));
	char pixels4[4] = {char(70), char(30), char(80), char(255)};
	Textures2D.add("nullBlack", Texture2D::createFromRaw(pixels4, 1, 1));
	char pixels5[4] = {char(255), char(255), char(255), char(255)};
	Textures2D.add("nullWhite", Texture2D::createFromRaw(pixels5, 1, 1));

	//program
	Programs.add("deferredLight", ShaderProgram::loadFromFile("data/shaders/depth.vert", "data/shaders/light.frag"));
	Programs.add("deferredModel", ShaderProgram::loadFromFile("data/shaders/standardDeferred.vert", "data/shaders/standardDeferred.frag"));
	Programs.add("ambientPass", ShaderProgram::loadFromFile("data/shaders/depth.vert", "data/shaders/ambientPass.frag"));
	Programs.add("blurPassVertical", ShaderProgram::loadFromFile("data/shaders/depth.vert", "data/shaders/blurPassVertical.frag"));
	Programs.add("blurPassHoritzontal", ShaderProgram::loadFromFile("data/shaders/depth.vert", "data/shaders/blurPassHoritzontal.frag"));
	Programs.add("textureToScreen", ShaderProgram::loadFromFile("data/shaders/depth.vert", "data/shaders/quad.frag"));
	Programs.add("blurMaskPass", ShaderProgram::loadFromFile("data/shaders/depth.vert", "data/shaders/blurMaskPass.frag"));
	Programs.add("depthShader", ShaderProgram::loadFromFile("data/shaders/depth.vert","data/shaders/depth.frag"));
	Programs.add("debug", ShaderProgram::loadFromFile("data/shaders/depth.vert","data/shaders/debug.frag"));
}

void SceneMain::update(float deltaTime) {
	PhysicsEngine::update(deltaTime);
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		VBE_LOG("FPS: " << fpsCount);
		Log::message() << pCount << Log::Flush;
		debugCounter--;
		fpsCount = 0;
	}
	Camera* cam = (Camera*)getGame()->getObjectByName("playerCam");
	if(Environment::getMouse()->isButtonPressed(Mouse::Left)) {
		GenericBody* b = new GenericBody();
		CircleCollider* p = new CircleCollider();
		p->setDensity(100.0f);
		p->setRadius(3.0f);
		p->setDType(Collider::Dynamic);
		p->setPosition(vec2f(cam->getWorldPos().x, cam->getWorldPos().y));
		b->set(p);
		b->addTo(this);
	}
	if(Environment::getKeyboard()->isKeyHeld(Keyboard::P)) {
		for(int i = 0; i < 30; ++i) {
			b2ParticleDef pd;
			pd.flags = b2_waterParticle;
			pd.color.Set(0, 0, 255, 255);
			pd.position.Set(((float(rand()%10000)/10000.0f)*2-1)*5+cam->getWorldPos().x, ((float(rand()%10000)/10000.0f)*2-1)*5+cam->getWorldPos().y);
			pd.lifetime = 1000000;
			psys->CreateParticle(pd);
		}
		pCount += 30;
	}
	if(Environment::getKeyboard()->isKeyPressed(Keyboard::M)) wall->removeAndDelete();
}
