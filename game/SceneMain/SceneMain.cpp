#include "SceneMain.hpp"
#include "DeferredContainer.hpp"
#include "BlurContainer.hpp"
#include "GenericBody.hpp"
#include "DeferredLight.hpp"
#include "Player.hpp"
#include "MyDebugDrawer.hpp"
#include "Manager.hpp"
#include "CeledoniousProfiler.hpp"
#include "CeledoniousSystem.hpp"

SceneMain::SceneMain() {
	this->setName("SCENE");

	loadResources();
	srand(Clock::getSeconds());

	//GL stuff..:
	glClearColor(0.02, 0.02, 0.02, 1);
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

	CeledoniousSystem* psys = new CeledoniousSystem();
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
	b->addCol(p);
	b->addTo(renderer);

	for(int i = 0; i < 10; ++i) {
		b = new GenericBody();
		p = new Physics::PolygonCollider();
		p->setAsBox(10.0f,1.0f);
		p->setDType(Physics::Collider::Static);
		p->setPosition(vec2f(5.0f*(i%2?-1:1),10.0f+5.0f*i));
		b->addCol(p);
		b->addTo(renderer);
	}

	b = new GenericBody();
	p = new Physics::PolygonCollider();
	p->setAsBox(1.0f,100.0f);
	p->setDType(Physics::Collider::Static);
	p->setPosition(vec2f(-10.0f,50.0f));
	b->addCol(p);
	b->addTo(renderer);

	b = new GenericBody();
	p = new Physics::PolygonCollider();
	p->setAsBox(1.0f,100.0f);
	p->setDType(Physics::Collider::Static);
	p->setPosition(vec2f(10.0f,50.0f));
	b->addCol(p);
	b->addTo(renderer);

	wall = b;

	DeferredLight* dl = new DeferredLight();
	dl->pos = vec3f(5.0f,5.0f,5.0f);
	dl->addTo(renderer);

	CeledoniousProfiler* profiler = new CeledoniousProfiler();
	profiler->addTo(this);
}

SceneMain::~SceneMain() {
	Textures2D.clear();
	Meshes.clear();
	Programs.clear();
	Physics::Engine::close();
}

void SceneMain::loadResources() {
	//meshes
	std::vector<Vertex::Attribute> elems = {
		Vertex::Attribute("a_position", Vertex::Attribute::Float, 3)
	};
	std::vector<vec3f> data = {
		vec3f(1, -1, 0), vec3f(1, 1, 0), vec3f(-1, 1, 0),
		vec3f(-1, -1, 0)
	};
	std::vector<unsigned int> indexes = {
		0, 1, 2, 3, 0, 2
	};
	MeshIndexed quad = MeshIndexed(Vertex::Format(elems));
	quad.setVertexData(&data[0], 6);
	quad.setIndexData(&indexes[0], 6);
	quad.setPrimitiveType(Mesh::TRIANGLES);
	Meshes.add("quad", std::move(quad));

	//textures
	char pixels[4] = {char(200), char(20), char(20), char(255)};
	Texture2D nullRed(vec2ui(1));
	nullRed.setData(pixels);
	Textures2D.add("nullRed", std::move(nullRed));
	char pixels2[4] = {char(20), char(200), char(20), char(255)};
	Texture2D nullGreen = Texture2D(vec2ui(1));
	nullRed.setData(pixels2);
	Textures2D.add("nullGreen", std::move(nullGreen));
	char pixels3[4] = {char(20), char(20), char(200), char(255)};
	Texture2D nullBlue = Texture2D(vec2ui(1));
	nullRed.setData(pixels3);
	Textures2D.add("nullBlue", std::move(nullBlue));
	char pixels4[4] = {char(70), char(30), char(80), char(255)};
	Texture2D nullBlack = Texture2D(vec2ui(1));
	nullRed.setData(pixels4);
	Textures2D.add("nullBlack", std::move(nullBlack));
	char pixels5[4] = {char(255), char(255), char(255), char(255)};
	Texture2D nullWhite = Texture2D(vec2ui(1));
	nullRed.setData(pixels5);
	Textures2D.add("nullWhite", std::move(nullWhite));

	//program
	Programs.add("deferredLight", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/light.frag")));
	Programs.add("deferredModel", ShaderProgram(Storage::openAsset("shaders/standardDeferred.vert"), Storage::openAsset("shaders/standardDeferred.frag")));
	Programs.add("ambientPass", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/ambientPass.frag")));
	Programs.add("blurPassVertical", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/blurPassVertical.frag")));
	Programs.add("blurPassHoritzontal", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/blurPassHoritzontal.frag")));
	Programs.add("textureToScreen", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/quad.frag")));
	Programs.add("blurMaskPass", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/blurMaskPass.frag")));
	Programs.add("depthShader", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/depth.frag")));
	Programs.add("debug", ShaderProgram(Storage::openAsset("shaders/depth.vert"), Storage::openAsset("shaders/debug.frag")));
}

void SceneMain::update(float deltaTime) {
	Physics::Engine::update(deltaTime);
	if(Keyboard::justPressed(Keyboard::M) && wall != nullptr) { wall->removeAndDelete(); wall = nullptr;}
	if(Keyboard::pressed(Keyboard::Escape) || Window::getInstance()->isClosing()) getGame()->isRunning = false;
}
