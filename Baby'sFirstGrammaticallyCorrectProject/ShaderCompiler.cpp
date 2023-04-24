#include "ShaderCompiler.h"

#include "FileUtility.h"
#include "yaml-cpp/yaml.h"
#include "LogMaster.h"
#include "ShaderProgram.h"

#include <sstream>
#include <fstream>

std::string RemoveFloatingVersionTag(const std::string& text)
{
	const std::string replace = "#version 450";
	const std::string with = "// [OMITTED BY COMPILER] #version 450\n";
	std::stringstream res;
	int i = 0;
	while (i < text.size())
	{
		int n = text.find(replace, i);
		if (n == -1)
		{
			n = text.size();
			res << text.substr(i, n);
			break;
		}
		else
		{
			res << text.substr(i, n);
			res << with;
			i = n + replace.size();
		}
	}
	return res.str();
}

std::string ParseBlock(const std::string& text, const std::string& dir, std::string* fname = nullptr)
{
	std::string res;
	if (text.substr(0, 3) == "$f ")
	{
		std::string filename = dir + text.substr(3);
		if (fname) *fname = filename;
		res = Utility::Files::LoadTextFromFile(filename);
	}
	else res = text;

	return RemoveFloatingVersionTag(res);
}

void WriteCompiledShader(const char* shaderName, const std::string& shader, std::stringstream& target)
{
	target << "\n" << shaderName << ":\n\"";
	int i = 0;
	while (i < shader.size())
	{
		int n = shader.find_first_of('\n', i);
		if (n == -1) n = shader.size();
		target << "\t" << shader.substr(i, n - i) << '\n';
		i = n + 1;
	}
	target << "\"";
}

void WriteCompilationResult(const std::string& shaderFileloc, const std::string& vert, const std::string& frag)
{
	std::string fileloc = "../Assets/Shaders/Compiled/" + shaderFileloc.substr(shaderFileloc.rfind('/') + 1) + " [COMPILATION OUTPUT].yaml";

	std::stringstream out;
	std::fstream fs{ fileloc.c_str(), std::ios::out };
	if (fs.is_open())
	{
		out << "#----------[Compilation Results of \"" << shaderFileloc << "\"]----------\n";
		WriteCompiledShader("Vertex", vert, out);
		WriteCompiledShader("Fragment", frag, out);
		fs << out.str();

		LogSuccess("Written Compilation Results of \"" << shaderFileloc << "\" to \"" << fileloc << "\"");
	}
	else
	{
		LogError("Failed to write Compilation Results of \"" << shaderFileloc << "\" to \"" << fileloc << "\"");
	}
	fs.close();
}

void ShaderCompiler::Compile(const std::string& fileloc, std::string& vert, std::string& frag, std::vector<ShaderProperty>& properties)
{
	std::string dir = fileloc.substr(0, fileloc.rfind('/') + 1);
	YAML::Node file = YAML::LoadFile(fileloc);

	YAML::Node propsNode = file["Properties"];
	YAML::Node incNode = file["Includes"];
	YAML::Node vertNode = file["Vertex"];
	YAML::Node fragNode = file["Fragment"];

	for (int i = 0; i < propsNode.size(); i++)
	{
		YAML::Node subnode = propsNode[i];
		properties.push_back(ShaderProperty(
			subnode["name"].as<std::string>(),
			subnode["ref"].as<std::string>(),
			subnode["type"].as<std::string>(),
			subnode["default"].as<std::string>(""))
		);
	}

	std::stringstream includes;
	for (int i = 0; i < incNode.size(); i++)
	{
		std::string incName = "Anonymous Include";
		std::string block = ParseBlock(incNode[i].as<std::string>(""), dir, &incName);
		includes << "\n\n// ----------[INCLUDE: " << incName << "]----------\n" << block;
	}
	vert += ParseBlock(vertNode.as<std::string>(""), dir);
	frag += ParseBlock(fragNode.as<std::string>(""), dir);

	vert = "#version 450\n" + includes.str() + vert;
	frag = "#version 450\n" + includes.str() + frag;

	WriteCompilationResult(fileloc, vert, frag);
}