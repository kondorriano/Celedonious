#include "SceneMain/SceneMain.hpp"

int main() {
	ContextSettings settings;
	settings.profile = ContextSettings::CoreProfile;
	settings.versionMajor = 4;
	settings.versionMinor = 3;
	Game* game = new Game(Window::DisplayMode::createWindowedMode(1000, 1000), settings);
	Window::getInstance()->setTitle("Celedonious");
	Mouse::setGrab(true);
	SceneMain* sc = new SceneMain();
	sc->addTo(game);
	game->run();
	delete game;
	return 42;
}
