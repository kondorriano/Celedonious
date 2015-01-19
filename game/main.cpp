#include "SceneMain/SceneMain.hpp"

int main() {
	Log::setFlags(Log::Timestamp | Log::StandardOut);
	ContextSettings settings;
	settings.profile = ContextSettings::CoreProfile;
	settings.versionMajor = 4;
	settings.versionMinor = 3;
	Game* game = new Game(Window::getFullscreenModes()[0], settings);
	Window::getInstance()->setTitle("Celedonious");
	Mouse::setGrab(false);
	SceneMain* sc = new SceneMain();
	sc->addTo(game);
	game->run();
	delete game;
	return 42;
}
