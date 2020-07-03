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

		std::vector<MeshInfo> Meshes;
		LightInfo LightInfo;
	};

	static RendererData s_RendererData;

	void Renderer::Init()
	{
		s_RendererData.PBRShaderTextured = CreateRef<Shader>("src/Resources/ShaderSource/PBR/vertex_textured_pbr.glsl", "src/Resources/ShaderSource/PBR/fragment_textured_pbr.glsl");
		s_RendererData.PBRShader = CreateRef<Shader>("src/Resources/ShaderSource/PBR/vertex_static_pbr.glsl", "src/Resources/ShaderSource/PBR/fragment_static_pbr.glsl");
		s_RendererData.CubemapShader = CreateRef<Shader>("src/Resources/ShaderSource/Cubemap/background_vertex.glsl", "src/Resources/ShaderSource/Cubemap/background_fragment.glsl");
	}

	void Renderer::BeginScene(Ref<Camera>& camera, Ref<Cubemap>& cubemap, const LightInfo& lightInfo)
	{
		s_RendererData.Camera = camera;
		s_RendererData.Cubemap = cubemap;
		s_RendererData.LightInfo = lightInfo;
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

				for (const Ref<Texture2D>& texture : mesh.Material->GetTextures())
				{
					switch (texture->GetType())
					{
					case TextureType::ALBEDO:
						texture->Bind(3);
						s_RendererData.PBRShaderTextured->SetInt("u_TextureAlbedo", 3);
						break;
					case TextureType::NORMAL:
						texture->Bind(4);
						s_RendererData.PBRShaderTextured->SetInt("u_TextureNormal", 4);
						break;
					case TextureType::METALLIC_SMOOTHNESS:
						texture->Bind(5);
						s_RendererData.PBRShaderTextured->SetInt("u_TextureMetallicSmooth", 5);
						break;
					case TextureType::AMBIENT_OCCLUSION:
						texture->Bind(6);
						s_RendererData.PBRShaderTextured->SetInt("u_TextureAmbient", 6);
						break;
					}
				}

				RendererAPI::DrawIndexed(mesh.VertexArray, 0);
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
			}
		}

		s_RendererData.Cubemap->BindEnvironmentMap(0);
		s_RendererData.CubemapShader->Bind();
		s_RendererData.CubemapShader->SetMat4("u_Projection", s_RendererData.Camera->GetProjectionMatrix());
		s_RendererData.CubemapShader->SetMat4("u_View", s_RendererData.Camera->GetViewMatrix());
		s_RendererData.CubemapShader->SetInt("u_EnvironmentMap", 0);

		RendererAPI::DrawIndexed(s_RendererData.Cubemap->GetVertexArray(), 0);

		s_RendererData.Meshes.clear();
	}

	void Renderer::Submit(Ref<Mesh>& mesh, const glm::mat4& modelMatrix)
	{
		s_RendererData.Meshes.push_back({ mesh->GetVertexArray(), mesh->GetMaterial(), modelMatrix });
	}

	void Renderer::Submit(Ref<Model>& model, uint16_t lod, uint16_t meshesPerLod)
	{
		for (const Mesh& mesh : model->GetMeshes())
		{
			s_RendererData.Meshes.push_back({ mesh.GetVertexArray(), mesh.GetMaterial(), model->GetModelMatrix() });
		}
	}
}