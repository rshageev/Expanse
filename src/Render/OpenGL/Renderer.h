#pragma once

#include "Render/IRenderer.h"

#include "Render/OpenGL/VertexArrayObject.h"
#include "Render/OpenGL/MaterialManager.h"

namespace Expanse::Render::GL
{
	class Renderer : public IRenderer
	{
	public:
		Renderer(Point window_size, Point framebuffer_size);

		void ClearFrame() override;
		void SetViewport(const Rect& rect) override;

		Material CreateMaterial(const std::string& file) override;
		Material CreateMaterial(Material material) override;
		void FreeMaterial(Material material) override;
		void SetMaterialParameter(Material material, std::string_view name, const MaterialParameterValue& value) override;

		Mesh CreateMesh() override;
		void FreeMesh(Mesh mesh) override;
		void SetMeshVertices(Mesh mesh, BufferData data, const VertexLayout& layout) override;
		void SetMeshIndices(Mesh mesh, BufferData data, size_t index_size) override;
		void SetMeshPrimitiveType(Mesh mesh, PrimitiveType prim_type) override;

		void Draw(Mesh mesh, Material material) override;

		Texture CreateTexture(const std::string& file) override;
		Texture CreateTexture(std::string_view name, const TextureDescription& tex_data) override;
		void FreeTexture(Texture texture) override;

		void Set2DMode() override;
	private:
		void LogOpenGLInfo();

		VertexArrayManager vertex_arrays;
		MaterialManager materials;

		struct ViewProjMatrices
		{
			glm::mat4 view{ 1.0f };
			glm::mat4 proj{ 1.0f };
		};
		ViewProjMatrices matrices;
	};
}