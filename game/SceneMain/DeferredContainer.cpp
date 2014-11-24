#include "DeferredContainer.hpp"
#include "Manager.hpp"

DeferredContainer::DeferredContainer() : gBuffer(nullptr), drawMode(Deferred) {
	setName("deferred");
	gBufferDepth.loadEmpty(vec2ui(1), TextureFormat::DEPTH_COMPONENT32);
	gBufferDepth.setFilter(GL_NEAREST, GL_NEAREST);
	gBufferColor0.loadEmpty(vec2ui(1), TextureFormat::RGB8);
	gBufferColor0.setFilter(GL_NEAREST, GL_NEAREST);
	gBufferColor1.loadEmpty(vec2ui(1), TextureFormat::RGBA16F);
	gBufferColor1.setFilter(GL_NEAREST, GL_NEAREST);
	gBuffer = new RenderTarget(1.0f);
	gBuffer->setTexture(RenderTarget::DEPTH, &gBufferDepth); //Z-BUFFER
	gBuffer->setTexture(RenderTarget::COLOR0, &gBufferColor0); //COLOR
	gBuffer->setTexture(RenderTarget::COLOR1, &gBufferColor1); //NORMAL, BRIGHTNESS, SPECULAR FACTOR

	quad = Meshes.get("quad");
}

DeferredContainer::~DeferredContainer() {
	delete gBuffer;
}

void DeferredContainer::update(float deltaTime) {
	ContainerObject::update(deltaTime);
}

void DeferredContainer::draw() const {
	//"The Screen". It may not be actually the screen since a upper container might be postprocessing
	const RenderTargetBase* screen = RenderTargetBase::getCurrent();
	//G BUFFER
	GL_ASSERT(glEnable(GL_DEPTH_TEST));
	GL_ASSERT(glDisable(GL_BLEND)); //no transparency whatsoever

	drawMode = Deferred;
	RenderTarget::bind(gBuffer);
	GL_ASSERT(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
	ContainerObject::draw();

	RenderTarget::bind(screen);
	GL_ASSERT(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

	//DEFERRED LIGHTS
	GL_ASSERT(glEnable(GL_BLEND));
	GL_ASSERT(glBlendFunc(GL_ONE, GL_ONE)); //additive
	GL_ASSERT(glDepthMask(GL_TRUE));
	GL_ASSERT(glDepthFunc(GL_ALWAYS));
	drawMode = Light;
	ContainerObject::draw();

	//AMBIENT LIGHT
	Programs.get("ambientPass")->uniform("MVP")->set(mat4f(1.0f));
	Programs.get("ambientPass")->uniform("color0")->set(getColor0());
	Programs.get("ambientPass")->uniform("color1")->set(getColor1());
	Programs.get("ambientPass")->uniform("depth")->set(getDepth());
	Programs.get("ambientPass")->uniform("invResolution")->set(vec2f(1.0f/Window::getInstance()->getSize().x, 1.0f/Window::getInstance()->getSize().y));
	quad->draw(Programs.get("ambientPass"));

	GL_ASSERT(glDepthFunc(GL_LEQUAL));
	GL_ASSERT(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); //forward rendering blending
	drawMode = Forward;
	ContainerObject::draw();
}

DeferredContainer::DrawMode DeferredContainer::getMode() const {
	return drawMode;
}

Texture2D *DeferredContainer::getColor0() const {
	return gBuffer->getTexture(RenderTarget::COLOR0);
}

Texture2D *DeferredContainer::getColor1() const {
	return gBuffer->getTexture(RenderTarget::COLOR1);
}

Texture2D* DeferredContainer::getDepth() const {
	return gBuffer->getTexture(RenderTarget::DEPTH);
}

