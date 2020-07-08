#include "oglpch.h"

#include "Renderer.h"

#include "RendererAPI.h"

namespace OpenGLRendering {

	struct MeshInfo
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
		Ref<Shader> ColorGradingShader;

		std::vector<MeshInfo> Meshes;
		LightInfo LightInfo;

		RendererStats Stats;
	};

	static RendererData s_RendererData;

	void Renderer::Init()
	{
		s_RendererData.PBRShaderTextured = CreateRef<Shader>("src/Resources/ShaderSource/PBR/vertex_textured_pbr.glsl", "src/Resources/ShaderSource/PBR/fragment_textured_pbr.glsl");
		s_RendererData.PBRShader = CreateRef<Shader>("src/Resources/ShaderSource/PBR/vertex_static_pbr.glsl", "src/Resources/ShaderSource/PBR/fragment_static_pbr.glsl");
		s_RendererData.CubemapShader = CreateRef<Shader>("src/Resources/ShaderSource/Cubemap/background_vertex.glsl", "src/Resources/ShaderSource/Cubemap/background_fragment.glsl");
		s_RendererData.ColorGradingShader = CreateRef<Shader>("src/Resources/ShaderSource/PostProcessing/color_grading_vertex.glsl", "src/Resources/ShaderSource/PostProcessing/color_grading_fragment.glsl");
	}

	void Renderer::BeginScene(Ref<Camera>& camera, Ref<Cubemap>& cubemap, const LightInfo& lightInfo)
	{
		s_RendererData.Camera = camera;
		s_RendererData.Cubemap = cubemap;
		s_RendererData.LightInfo = lightInfo;

		s_RendererData.Stats.VertexCount = 0;
		s_RendererData.Stats.FaceCount = 0;
		s_RendererData.Stats.DrawCalls = 0;
	}

	void Renderer::EndScene()
	{
		RendererAPI::Clear();

		s_RendererData.Cubemap->BindIrradianceMap(0);
		s_RendererData.Cubemap->BindPrefilterMap(1);
		s_RendererData.Cubemap->BindBrdfLutTexture(2);

		for (const MeshInfo& mesh : s_RendererData.Meshes)
		{
			if (mesh.Material->IsUsingTextures())
			{
				s_RendererData.PBRShaderTextured->Bind();
				s_RendererData.PBRShaderTextured->SetMat4("u_Projection", s_RendererData.Camera->GetProjectionMatrix());
				s_RendererData.PBRShaderTextured->SetMat4("u_View", s_RendererData.Camera->GetViewMatrix());
				s_RendererData.PBRShaderTextured->SetMat4("u_Model", mesh.ModelMatrix);
				s_RendererData.PBRShaderTextured->SetFloat3("u_LightPos", s_RendererData.LightInfo.LightPos);
				s_RendererData.PBRShaderTextured->SetFloat3("u_LightColor", s_RendererData.LightInfo.LightColor);
				s_RendererData.PBRShaderTextured->SetFloat3("u_CameraPos", s_RendererData.Camera->GetPosition());

				s_RendererData.PBRShaderTextured->SetInt("u_IrradianceMap", 0);
				s_RendererData.PBRShaderTextured->SetInt("u_PrefilterMap", 1);
				s_RendererData.PBRShaderTextured->SetInt("u_BrdfLutTexture", 2);

				const std::unordered_map<TextureType, Ref<Texture2D>>& textures = mesh.Material->GetTextures();
				
				if (textures.find(TextureType::ALBEDO) != textures.end())
				{
					textures.at(TextureType::ALBEDO)->Bind(3);
					s_RendererData.PBRShaderTextured->SetInt("u_TextureAlbedo", 3);
				}

				if (textures.find(TextureType::NORMAL) != textures.end())
				{
					textures.at(TextureType::NORMAL)->Bind(4);
					s_RendererData.PBRShaderTextured->SetInt("u_TextureNormal", 4);
				}

				if (textures.find(TextureType::METALLIC_SMOOTHNESS) != textures.end())
				{
					textures.at(TextureType::METALLIC_SMOOTHNESS)->Bind(5);
					s_RendererData.PBRShaderTextured->SetInt("u_TextureMetallicSmooth", 5);
				}

				if (textures.find(TextureType::AMBIENT_OCCLUSION) != textures.end())
				{
					textures.at(TextureType::AMBIENT_OCCLUSION)->Bind(6);
					s_RendererData.PBRShaderTextured->SetInt("u_TextureAmbient", 6);
				}

				RendererAPI::DrawIndexed(mesh.VertexArray, 0);
				s_RendererData.Stats.DrawCalls += 1;
			}
			else
			{
				s_RendererData.PBRShader->Bind();
				s_RendererData.PBRShader->SetMat4("u_Projection", s_RendererData.Camera->GetProjectionMatrix());
				s_RendererData.PBRShader->SetMat4("u_View", s_RendererData.Camera->GetViewMatrix());
				s_RendererData.PBRShader->SetMat4("u_Model", mesh.ModelMatrix);
				s_RendererData.PBRShader->SetFloat3("u_LightPos", s_RendererData.LightInfo.LightPos);
				s_RendererData.PBRShader->SetFloat3("u_LightColor", s_RendererData.LightInfo.LightColor);
				s_RendererData.PBRShader->SetFloat3("u_CameraPos", s_RendererData.Camera->GetPosition());

				s_RendererData.PBRShader->SetInt("u_IrradianceMap", 0);
				s_RendererData.PBRShader->SetInt("u_PrefilterMap", 1);
				s_RendererData.PBRShader->SetInt("u_BrdfLutTexture", 2);

				s_RendererData.PBRShader->SetFloat3("u_Albedo", mesh.Material->GetAlbedo());
				s_RendererData.PBRShader->SetFloat("u_Roughness", mesh.Material->GetRoughness());
				s_RendererData.PBRShader->SetFloat("u_Metallic", mesh.Material->GetMetallic());
				s_RendererData.PBRShader->SetFloat("u_Ambient", mesh.Material->GetAmbientOcclusion());

				RendererAPI::DrawIndexed(mesh.VertexArray, 0);
				s_RendererData.Stats.DrawCalls += 1;
			}
		}

		s_RendererData.Cubemap->BindEnvironmentMap(0);
		s_RendererData.CubemapShader->Bind();
		s_RendererData.CubemapShader->SetMat4("u_Projection", s_RendererData.Camera->GetProjectionMatrix());
		s_RendererData.CubemapShader->SetMat4("u_View", s_RendererData.Camera->GetViewMatrix());
		s_RendererData.CubemapShader->SetInt("u_EnvironmentMap", 0);

		RendererAPI::DrawIndexed(s_RendererData.Cubemap->GetVertexArray(), 0);
		s_RendererData.Stats.DrawCalls += 1;

		s_RendererData.Meshes.clear();
	}

	void Renderer::Submit(Ref<Mesh>& mesh, const glm::mat4& modelMatrix)
	{
		s_RendererData.Meshes.push_back({ mesh->GetVertexArray(), mesh->GetMaterial(), modelMatrix });
		s_RendererData.Stats.VertexCount += mesh->GetVertexCount();
		s_RendererData.Stats.FaceCount += mesh->GetFaceCount();
	}

	void Renderer::Submit(Ref<Model>& model, uint16_t lod, uint16_t meshesPerLod)
	{
		for (unsigned int i = lod * meshesPerLod; i < (lod + 1) * meshesPerLod && i < model->GetMeshes().size(); i++)
		{
			const Mesh& mesh = model->GetMeshes()[i];
			s_RendererData.Meshes.push_back({ mesh.GetVertexArray(), mesh.GetMaterial(), model->GetModelMatrix() });
			s_RendererData.Stats.VertexCount += mesh.GetVertexCount();
			s_RendererData.Stats.FaceCount += mesh.GetFaceCount();
		}
	}

	void Renderer::Submit(Ref<Model>& model)
	{
		for (const Mesh& mesh : model->GetMeshes())
		{
			s_RendererData.Meshes.push_back({ mesh.GetVertexArray(), mesh.GetMaterial(), model->GetModelMatrix() });
			s_RendererData.Stats.VertexCount += mesh.GetVertexCount();
			s_RendererData.Stats.FaceCount += mesh.GetFaceCount();
		}
	}

	void Renderer::ColorGrade(const glm::vec4& color)
	{

	}

	const RendererStats& Renderer::GetStatistics()
	{
		return s_RendererData.Stats;
	}
}