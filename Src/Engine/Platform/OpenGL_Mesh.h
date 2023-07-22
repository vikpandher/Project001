#pragma once



namespace Project001
{
    struct MeshVertex;

    class OpenGL_Mesh
    {
    public:
        OpenGL_Mesh(
            const MeshVertex* meshVertexPtr,
            unsigned int meshVertexCount,
            const unsigned int* meshIndexPtr,
            unsigned int meshIndexCount);

        ~OpenGL_Mesh();

        void UpdateVertexArrayObject(unsigned int instanceBufferId);

        void Render(unsigned int instanceCount);

    protected:
        unsigned int indexCount_;

        unsigned int vertexArrayId_;
        unsigned int vertexBufferId_;
        unsigned int indexBufferId_;
    };
}
