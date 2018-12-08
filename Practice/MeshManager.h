#pragma once

#include <unordered_map>
#include <GLFW/glfw3.h>


//w/e, we'll reorg later
struct Mesh
{
    GLuint VAO;
    GLuint VBO; // Just need to store the ID so I can cleanup later
    float* verts; //store for debug? might be useful later? (I mean, it has to stick around in memory, but maybe we store on this object rather than some random place on the stack)
};

class MeshManager
{
public:
    void CreateMesh(const std::string& name, float* vertData, int size, bool hasUVs);
    void DeleteMeshes();
    //prob return a bool if lookup was successful and return actual value via outParam
    GLuint GetMesh(const std::string& name);

private:
    std::unordered_map<std::string, Mesh> mMeshes;
};