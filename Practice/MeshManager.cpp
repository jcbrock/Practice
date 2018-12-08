#include <glad/glad.h>

#include "MeshManager.h"


void MeshManager::CreateMesh(const std::string& name, float* vertData, int size, bool hasUVs)
{
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Setup VAO (attributes for this VBO, so in the future we just bind to this, draw object, and then unbind)
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertData, GL_STATIC_DRAW);

    int sizeOfXYZ = 3;
    int sizeOfUV = 2;
    int strideSize = sizeOfXYZ;
    if (hasUVs)
    {
        strideSize += sizeOfUV;
    }
    // position attribute
    glVertexAttribPointer(0, sizeOfXYZ, GL_FLOAT, GL_FALSE, strideSize * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    if (hasUVs)
    {
        // texture coord attribute
        glVertexAttribPointer(1, sizeOfUV, GL_FLOAT, GL_FALSE, strideSize * sizeof(float), (void*)(sizeOfXYZ * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    // End Setup VAO

    // I just need to save off the ID
    mMeshes[name] = { VAO, VBO }; //TODO - look into this type of init
}

void MeshManager::DeleteMeshes()
{
    for (auto& pair : mMeshes)
    {
        Mesh& m = pair.second;
        glDeleteVertexArrays(1, &m.VAO);
        glDeleteBuffers(1, &m.VBO);
    }

    mMeshes.clear();
}

//prob return a bool if lookup was successful and return actual value via outParam
GLuint MeshManager::GetMesh(const std::string& name)
{
    return mMeshes[name].VAO;
}