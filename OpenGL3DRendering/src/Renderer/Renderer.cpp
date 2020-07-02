#include "oglpch.h"

#include "Renderer.h"

namespace OpenGLRendering {

	struct RenderInfo
	{
		Ref<VertexArray> VertexArray;
		Ref<Material> Material;

		glm::mat4 ModelMatrix;
	};

	struct RendererData
	{
		Ref<Camera> Camera;
		Ref<Cubemap> Cubemap;

		Ref<Shader> PBRShaderTextured;
		Ref<Shader> PBRShader;
		Ref<Shader> CubemapShader;

		std::vector<RenderInfo> Meshes;
	};

	static RendererData s_RendererData;

	void Renderer::Init()
	{
		s_RendererData.PBRShaderTextured = CreateRef<Shader>("src/Resources/ShaderSource/vertex_pbr.glsl", "src/Resources/ShaderSource/fragment_pbr.glsl");
		s_RendererData.CubemapShader = CreateRef<Shader>("src/Resources/ShaderSource/cubemap_background_vertex.glsl", "src/Resources/ShaderSource/cubemap_background_fragment.glsl");
	}

	void Renderer::BeginScene(Ref<Camera> camera, Ref<Cubemap> cubemap)
	{
		s_RendererData.Camera = camera;
		s_RendererData.Cubemap = cubemap;
	}

	void Renderer::EndScene()
	{
		for (const RenderInfo& mesh : s_RendererData.Meshes)
		{

		}
	}

	void Renderer::Submit(Ref<Mesh> mesh, const glm::mat4& modelMatrix)
	{
		s_RendererData.Meshes.push_back({ mesh->GetVertexArray(), mesh->GetMaterial(), modelMatrix });
	}

	void Renderer::Submit(Ref<Model> model, uint16_t lod, uint16_t meshesPerLod)
	{
		for (const Mesh& mesh : model->GetMeshes())
		{
			s_RendererData.Meshes.push_back({ mesh.GetVertexArray(), mesh.GetMaterial(), model->GetModelMatrix() });
		}
	}
}