#pragma once

class VertexArray
{
public:
	VertexArray(const float* verts, unsigned int numVerts,
		const unsigned int* indices, unsigned int numIndices);
	~VertexArray();

	// active this vertex array (so we can draw it)
	void SetActive();

	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }

private:
	// how many vertices in the vertex buffer?
	unsigned int mNumVerts;
	// how many indices in the index buffer
	unsigned int mNumIndices;
	// open gl ID of the vertex buffer
	unsigned int mVertexBuffer;
	// open gl id of the index buffer
	unsigned int mIndexBuffer;
	// open gl of the vertex array object
	unsigned int mVertexArray;

};