#pragma once

#include "Render/ResourceHandles.h"

namespace Expanse::Render::GL
{
	struct Shader : public Handle {};

	struct ShaderUniformInfo
	{
		std::string name;
		GLint location;
	};

	class ShaderManager
	{
	public:
		ShaderManager() = default;
		~ShaderManager();
		ShaderManager(const ShaderManager&) = delete;
		ShaderManager& operator=(const ShaderManager&) = delete;

		Shader Create(const std::string& file);
		void Free(Shader program);
		void Use(Shader program);

		std::vector<ShaderUniformInfo> GetShaderUniformsInfo(Shader shader);
		std::vector<std::string> GetShaderUniformBlocks(Shader shader);

		void BindUniformBlock(Shader shader, GLuint binding_point, const std::string& name);
	private:
		struct ShaderResource
		{
			GLuint id = 0;
			size_t use_count = 0;
			std::string name;
		};
		std::vector<ShaderResource> shaders;
		GLuint current_shader = 0;
	};
}