#pragma once

#include <memory>
#include <string>
#include <unordered_map>

class Shader;
class Texture;

class Assets
{
public:
	static std::shared_ptr<Shader> GetShader(const std::string &path);
	static std::shared_ptr<Texture> GetTexture(const std::string &path);

private:
	inline static std::unordered_map<std::string, std::shared_ptr<Shader>>
		shaders;
	inline static std::unordered_map<std::string, std::shared_ptr<Texture>>
		textures;
};
