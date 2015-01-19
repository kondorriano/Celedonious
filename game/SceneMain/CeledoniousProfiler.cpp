#include "CeledoniousProfiler.hpp"

vec2f CeledoniousProfiler::playerPos = vec2f(0.0f);
int CeledoniousProfiler::particleCount = 0;
float CeledoniousProfiler::waterCounter = 0;

CeledoniousProfiler::CeledoniousProfiler() {
	Log::message() << "WOLOLO esto es el log :3" << Log::Flush; //test
}

CeledoniousProfiler::~CeledoniousProfiler() {
}

void CeledoniousProfiler::renderCustomInterface() const {
	ImGui::Begin("Data", nullptr, ImVec2(0.18f*Window::getInstance()->getSize().x, 0.21f*Window::getInstance()->getSize().y), 0.9f, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(0.78f*Window::getInstance()->getSize().x, 0.05f*Window::getInstance()->getSize().y), ImGuiSetCondition_FirstUseEver);
	ImGui::Columns(2, nullptr, true);
	ImGui::SetColumnOffset(0,0);
	ImGui::SetColumnOffset(1,0.4f*0.18f*Window::getInstance()->getSize().x);
	ImGui::Text("Player Pos"); ImGui::NextColumn();
	ImGui::Text("vec2f(%s, %s)", Utils::toString(playerPos.x,2,2,true).c_str(), Utils::toString(playerPos.y,2,2,true).c_str()); ImGui::NextColumn();
	ImGui::Text("Water Counter"); ImGui::NextColumn();
	ImGui::Text(Utils::toString(waterCounter).c_str()); ImGui::NextColumn();
	ImGui::Text("Particle Count"); ImGui::NextColumn();
	ImGui::Text(Utils::toString(particleCount).c_str()); ImGui::NextColumn();
	//PARA AÑADIR MAS COSAS: Copia esto, descomentalo, y sustituye "Particle Count" y particleCount por lo que sea
	//ImGui::Text("Particle Count"); ImGui::NextColumn();
	//ImGui::Text(Utils::toString(particleCount).c_str()); ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();
	ImGui::Text("While showing the interface,\nthe mouse will be visible.\nHover the profiler graphs for\nmore info");
	ImGui::End();
}
