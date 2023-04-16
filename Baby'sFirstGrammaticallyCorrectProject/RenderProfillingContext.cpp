#include "RenderProfillingContext.h"

#include <vector>
#include <imgui.h>

std::vector<RenderProfillingContext> working;
std::vector<RenderProfillingContext> compiled;

RenderProfillingContext::RenderProfillingContext(const std::string& name) : name(name)
{
	durationMillisecconds = 0.0f;
	start = std::chrono::high_resolution_clock::now();
}

void RenderProfillingContext::Stop()
{
	auto end = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	renderTime = ms.count() / 1000.0f;
}

void RenderProfillingContext::StopLast()
{
	if (working.size() > 0) working[working.size() - 1].Stop();
}

void RenderProfillingContext::Start(const std::string& name)
{
	StopLast();
	working.emplace_back(name);
}

void RenderProfillingContext::Finalize()
{
	StopLast();
	compiled = working;
	working.clear();
}

void RenderProfillingContext::RenderGUI()
{
	if (!ImGui::CollapsingHeader("Render Profiler")) return;

	ImGui::Indent();

	for (const RenderProfillingContext& sample : compiled)
	{
		ImGui::Text((sample.name + "\tRender Time: " + std::to_string(sample.renderTime) + " ms").c_str());
	}

	ImGui::Unindent();
}
