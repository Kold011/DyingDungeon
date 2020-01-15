#include "Mesh.h"
#include "Buffer.h"
#include "RenderDevice.h"

namespace Odyssey
{
	Mesh::Mesh(std::shared_ptr<RenderDevice> renderDevice, std::vector<Vertex> vertexList, std::vector<unsigned int> indexList)
	{
		mVertexBuffer = renderDevice->createBuffer(BufferBindFlag::VertexBuffer, static_cast<size_t>(vertexList.size()),
			static_cast<UINT>(sizeof(Vertex)), (void*)vertexList.data());

		mIndexBuffer = renderDevice->createBuffer(BufferBindFlag::IndexBuffer, static_cast<size_t>(indexList.size()),
			static_cast<UINT>(sizeof(unsigned int)), (void*)indexList.data());
		mNumberOfIndices = static_cast<int>(indexList.size());
	}

	void Mesh::bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Bind the vertex and index buffer to the rendering pipeline.
		mVertexBuffer->bind(context);
		mIndexBuffer->bind(context);
	}

	void Mesh::unbind(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context)
	{
		// Bind the vertex and index buffer to the rendering pipeline.
		mVertexBuffer->unbind(context, 0, ShaderType::VertexShader);
		mIndexBuffer->unbind(context, 0, ShaderType::VertexShader);
	}

	Buffer* Mesh::getVertexBuffer()
	{
		return mVertexBuffer.get();
	}

	Buffer* Mesh::getIndexBuffer()
	{
		return mIndexBuffer.get();
	}
	
	const int Mesh::getNumberOfIndices()
	{
		return mNumberOfIndices;
	}

	void Mesh::setName(std::string name)
	{
		mName = name;
	}

	void Mesh::setNumberOfindices(int numIndices)
	{
		mNumberOfIndices = numIndices;
	}
}