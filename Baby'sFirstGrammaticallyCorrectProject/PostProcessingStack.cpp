#include "PostProcessingStack.h"

#include "AssetDatabase.h"
#include "Submesh.h"

Material& CopyPassMaterial();
Submesh& Quad();

void PostProcessingStack::Init()
{

}

void PostProcessingStack::Bind()
{
	SwapBuffers();
}

void PostProcessingStack::Draw()
{
	Blit(&Dest(), nullptr);
}

void PostProcessingStack::Unbind()
{

}

void PostProcessingStack::SwapBuffers()
{
	swapBuffers = !swapBuffers;
	Source().Unbind();
	Dest().Bind();
}

void PostProcessingStack::Blit(Framebuffer* src, Framebuffer* dst, Material* material)
{
	if (!material) material = &CopyPassMaterial();
	Framebuffer* bound = Framebuffer::Bound();

	dst->Bind();
	material->SetTexture("screenTexture", &src->RenderTexture());

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	material->Bind();
	Quad().Draw();
	material->Unbind();

	dst->Unbind();
	bound->Bind();
}

Material copyPassMaterial;
bool isCopyPassMaterialInitalized = false;

Material& CopyPassMaterial()
{
	if (!isCopyPassMaterialInitalized)
	{
		copyPassMaterial.SetShader("CopyPass.yaml");
		isCopyPassMaterialInitalized = true;
	}
	return copyPassMaterial;
}

Submesh* quad;

std::vector<VertexData> quadVerts =
{
	{{-1.0f, -1.0f, 0.0f, 1.0f}, {}, {-1.0f, -1.0f}, {}},
	{{-1.0f, 1.0f, 0.0f, 1.0f}, {}, {-1.0f, -1.0f}, {}},
	{{1.0f, 1.0f, 0.0f, 1.0f}, {}, {-1.0f, -1.0f}, {}},
	{{1.0f, -1.0f, 0.0f, 1.0f}, {}, {-1.0f, -1.0f}, {}},
};

std::vector<MeshIndex> quadIndices =
{
	0, 1, 2, 2, 3, 0
};

Submesh& Quad()
{
	if (!quad)
	{
		quad = new Submesh(0, quadVerts, quadIndices);
	}
	return *quad;
}