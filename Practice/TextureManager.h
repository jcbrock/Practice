#pragma once

//TODO - probably make a generic singleton template at some point
//TODO - someday probably split OpenGL stuff from file loading...

class TextureManager
{
public:
    static void Create();
    static void Destroy();
    static bool IsValid();

    static TextureManager& Instance();

    //TextureManager specific funcs
    unsigned int LoadFile(const char* fileName, bool hasAlphaChannel);

protected:
    static TextureManager* sInstance;

private: 
    TextureManager() = default;
    ~TextureManager() = default;

    unsigned int mTextureCount = 0;

    // Effective Modern C++ book: that deleted functions should generally be public as it results in better error messages
    // due to the compilers behavior to check accessibility before deleted status
public:
    TextureManager(TextureManager const&) = delete;
    void operator=(TextureManager const&) = delete;

};