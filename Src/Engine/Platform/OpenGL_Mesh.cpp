#include "OpenGL_Mesh.h"

#include "Engine/MeshVertex.h"
#include "Engine/RenderData.h"

#include "glad/glad.h"



namespace Project001
{
    // public ------------------------------------------------------------------

    OpenGL_Mesh::OpenGL_Mesh(
        const MeshVertex* meshVertexPtr,
        unsigned int meshVertexCount,
        const unsigned int* meshIndexPtr,
        unsigned int meshIndexCount)
        : indexCount_(meshIndexCount)
        , vertexArrayId_((unsigned int)-1)
    {
        glGenBuffers(1, &vertexBufferId_);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData) * meshVertexCount, NULL, GL_STATIC_DRAW);
        VertexData* mappedVertexDataPtr = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        for (unsigned int i = 0; i < meshVertexCount; ++i)
        {
            mappedVertexDataPtr[i].position = meshVertexPtr[i].position;
            mappedVertexDataPtr[i].textureCoordinate = meshVertexPtr[i].textureCoordinate;
            mappedVertexDataPtr[i].normal = meshVertexPtr[i].normal;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenBuffers(1, &indexBufferId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * meshIndexCount, NULL, GL_DYNAMIC_DRAW);
        unsigned int* mappedIndexDataPtr = (unsigned int*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
        for (unsigned int i = 0; i < meshIndexCount; ++i)
        {
            mappedIndexDataPtr[i] = meshIndexPtr[i];
        }
        glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    OpenGL_Mesh::~OpenGL_Mesh()
    {
        glDeleteVertexArrays(1, &vertexArrayId_);
        glDeleteBuffers(GL_ARRAY_BUFFER, &vertexBufferId_);
        glDeleteBuffers(GL_ELEMENT_ARRAY_BUFFER, &indexBufferId_);
    }

    void OpenGL_Mesh::UpdateVertexArrayObject(unsigned int instanceBufferId)
    {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vertexArrayId_);
        glGenVertexArrays(1, &vertexArrayId_);
        glBindVertexArray(vertexArrayId_);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId_);

        const GLuint positionAttributeIndex = 0;
        const GLuint textureCoordinateAttributeIndex = 1;
        const GLuint normalAttributeIndex = 2;

        unsigned long long attributeOffset = 0;

        glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(textureCoordinateAttributeIndex, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);
        attributeOffset += sizeof(glm::vec2);

        glVertexAttribPointer(normalAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)attributeOffset);

        glBindBuffer(GL_ARRAY_BUFFER, instanceBufferId);

        const GLuint colorAttributeIndex = 3;
        const GLuint textureUnitAttributeIndex = 4;
        const GLuint specularUnitAttributeIndex = 5;
        const GLuint shininessAttributeIndex = 6;
        const GLuint scaleAttributeIndex = 7;
        const GLuint translationAttributeIndex = 8;
        const GLuint orientationAttributeIndex = 9;
        const GLuint litAttributeIndex = 10;

        attributeOffset = 0;

        glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);
        attributeOffset += sizeof(glm::vec4);

        glVertexAttribPointer(textureUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(specularUnitAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(shininessAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);
        attributeOffset += sizeof(float);

        glVertexAttribPointer(scaleAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(translationAttributeIndex, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);
        attributeOffset += sizeof(glm::vec3);

        glVertexAttribPointer(orientationAttributeIndex, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);
        attributeOffset += sizeof(glm::quat);

        glVertexAttribPointer(litAttributeIndex, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceData), (void*)attributeOffset);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void OpenGL_Mesh::Render(unsigned int instanceCount)
    {
        glBindVertexArray(vertexArrayId_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId_);

        glDrawElementsInstanced(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0, instanceCount);

        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}