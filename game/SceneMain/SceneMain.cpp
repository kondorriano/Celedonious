#include "SceneMain.hpp"
#include "DeferredContainer.hpp"
#include "BlurContainer.hpp"
#include "GenericBody.hpp"
#include "DeferredLight.hpp"
#include "Player.hpp"
#include "MyDebugDrawer.hpp"
#include "Manager.hpp"

SceneMain::SceneMain() : debugCounter(0.0f), fpsCount(0) {
	this->setName("SCENE");

	loadResources();
	srand(Clock::getSeconds());

	//GL stuff..:
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE); //enable backface culling
	glCullFace(GL_BACK);
	glPointSize(4.0f);

	Physics::Engine::init(vec2f(0.0f, -9.8f), 60.0f, 2, 8, 2);

	BlurContainer* blur = new BlurContainer();
	blur->addTo(this);

	DeferredContainer* renderer = new DeferredContainer();
	renderer->addTo(blur);

	MyDebugDrawer* dbg = new MyDebugDrawer();
	dbg->addTo(renderer);

	Player* player = new Player();
	player->addTo(renderer);

	Physics::ParticleSystem* psys = new Physics::ParticleSystem();
	psys->setRadius(0.1f);
	psys->setGravityScale(1.0f);
	psys->setStaticPressureIterations(8);
	psys->setName("psys");
	psys->addTo(renderer);

	GenericBody* b = new GenericBody();
	Physics::PolygonCollider* p = new Physics::PolygonCollider();
	p->setAsBox(20.0f,1.0f);
	p->setDType(Physics::Collider::Static);
	p->setPosition(vec2f(0.0f, -10.0f));
	p->setFriction(1.0f);
	p->setRestitution(0.1f);
	b->set(p);
	b->addTo(renderer);

	for(int i = 0; i < 10; ++i) {
		b = new GenericBody();
		p = new Physics::PolygonCollider();
		p->setAsBox(10.0f,1.0f);
		p->setDType(Physics::Collider::Static);
		p->setPosition(vec2f(5.0f*(i%2?-1:1),10.0f+5.0f*i));
		b->set(p);
		b->addTo(renderer);
	}

	b = new GenericBody();
	p = new Physics::PolygonCollider();
	p->setAsBox(1.0f,100.0f);
	p->setDType(Physics::Collider::Static);
	p->setPosition(vec2f(-10.0f,50.0f));
	b->set(p);
	b->addTo(renderer);

	b = new GenericBody();
	p = new Physics::PolygonCollider();
	p->setAsBox(1.0f,100.0f);
	p->setDType(Physics::Collider::Static);
	p->setPosition(vec2f(10.0f,50.0f));
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
	Physics::Engine::close();
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
	MeshBase* quad = new Mesh(Vertex::Format(elems));
	quad->setVertexData(&data[0], 6);
	quad->setPrimitiveType(Mesh::TRIANGLES);
	Meshes.add("quad", quad);
	MeshBase* monkey = OBJLoader::loadFromOBJStandard(Storage::openAsset("meshes/monkey.obj"), MeshBase::STATIC);
	Meshes.add("monkey", monkey);

	//textures	//textures
	char pixels[4] = {char(200), char(20), char(20), char(255)};
	Texture2D* nullRed = new Texture2D();
	nullRed->loadFromRaw(pixels, vec2ui(1));
	Textures2D.add("nullRed", nullRed);
	char pixels2[4] = {char(20), char(200), char(20), char(255)};
	Texture2D* nullGreen = new Texture2D();
	nullRed->loadFromRaw(pixels2, vec2ui(1));
	Textures2D.add("nullGreen", nullGreen);
	char pixels3[4] = {char(20), char(20), char(200), char(255)};
	Texture2D* nullBlue = new Texture2D();
	nullRed->loadFromRaw(pixels3, vec2ui(1));
	Textures2D.add("nullBlue", nullBlue);
	char pixels4[4] = {char(70), char(30), char(80), char(255)};
	Texture2D* nullBlack = new Texture2D();
	nullRed->loadFromRaw(pixels4, vec2ui(1));
	Textures2D.add("nullBlack", nullBlack);
	char pixels5[4] = {char(255), char(255), char(255), char(255)};
	Texture2D* nullWhite = new Texture2D();
	nullRed->loadFromRaw(pixels5, vec2ui(1));
	Textures2D.add("nullWhite", nullWhite);

	//program
	Programs.add("deferredLight", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/light.frag")));
	Programs.add("deferredModel", ShaderProgram::load(Storage::openAsset("shaders/standardDeferred.vert"), Storage::openAsset("shaders/standardDeferred.frag")));
	Programs.add("ambientPass", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/ambientPass.frag")));
	Programs.add("blurPassVertical", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/blurPassVertical.frag")));
	Programs.add("blurPassHoritzontal", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/blurPassHoritzontal.frag")));
	Programs.add("textureToScreen", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/quad.frag")));
	Programs.add("blurMaskPass", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/blurMaskPass.frag")));
	Programs.add("depthShader", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/depth.frag")));
	Programs.add("debug", ShaderProgram::load(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/debug.frag")));
}

void SceneMain::update(float deltaTime) {
	Physics::Engine::update(deltaTime);
	++fpsCount;
	debugCounter += deltaTime;
	if (debugCounter > 1) {
		VBE_LOG("FPS: " << fpsCount);
		Physics::ParticleSystem* psys = (Physics::ParticleSystem*)getGame()->getObjectByName("psys");
		Log::message() << psys->getParticleCount() <<  " " << fpsCount << Log::Flush;
		debugCounter--;
		fpsCount = 0;
	}
	if(Keyboard::justPressed(Keyboard::M)) wall->removeAndDelete();
	if(Keyboard::pressed(Keyboard::Escape) || Window::getInstance()->isClosing()) getGame()->isRunning = false;
}
