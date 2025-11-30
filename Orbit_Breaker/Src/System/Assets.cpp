#include "Assets.hpp"

#include "../Render/Shader.hpp"
#include "../Render/Texture.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

std::shared_ptr<Shader> Assets::GetShader(const std::string &path)
{
	auto full_path = std::filesystem::absolute(path).string();
	if (shaders.contains(full_path))
	{
		return shaders[full_path];
	}

	auto shader = std::make_shared<Shader>(path + ".vs", path + ".fs");
	shaders[full_path] = shader;
	return shader;
}

std::shared_ptr<Texture> Assets::GetTexture(const std::string &path)
{
	auto full_path = std::filesystem::absolute(path).string();

	if (textures.contains(full_path))
	{
		return textures[full_path];
	}

	auto texture = std::make_shared<Texture>();
	texture->Init(path);
	textures[full_path] = texture;
	return texture;
}
