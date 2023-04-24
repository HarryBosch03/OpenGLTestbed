#pragma once

#include <string>
#include "AssetDatabase.h"
#include "imgui/imgui.h"

class Material;

namespace Utility
{
	namespace Inspector
	{
		int& WorkingID();
		const char* AppendID(const std::string& text);

		void DrawMaterial(Material& material);

		template<typename T>
		void AssetDropdown(T*& asset, const std::string& varName, const std::string& idAppend = "", const std::string& fallback = "")
		{
			std::string first = "Change " + varName + "... (" + (asset ? asset->FileName() : "null") + ")";

			std::vector<std::string> assets = AssetDatabase::GetType<T>();
			const char** items = new const char* [assets.size() + 2];
			items[0] = first.c_str();
			items[1] = "None";
			for (int i = 0; i < assets.size(); i++)
			{
				items[i + 2] = assets[i].c_str();
			}

			int item = 0;
			if (ImGui::Combo(("##" + idAppend + std::to_string((unsigned int)asset)).c_str(), &item, items, assets.size() + 2))
			{
				if (item > 1)
				{
					asset = AssetDatabase::Get<T>(items[item]);
				}
				if (item == 1)
				{
					asset = nullptr;
				}
			}

			delete[] items;
		}
	}
}