#include <VertexBuffer.h>

using std::vector;
using Microsoft::WRL::ComPtr;

VertexBuffer::VertexBuffer() : stride(0), count(0), buffer(nullptr), shader(nullptr) {
}

VertexBuffer::~VertexBuffer() {
	if (buffer)
		buffer->Release();
	if (shader)
		shader->Release();
}

void VertexBuffer::Bind() {
	Renderer* renderer = Renderer::GetInstance();
	const UINT offset = 0u;
	renderer->GetContext()->IASetVertexBuffers(0u, 1u, &buffer, &stride, &offset);
}

UINT VertexBuffer::GetCount() {
	return count;
}
