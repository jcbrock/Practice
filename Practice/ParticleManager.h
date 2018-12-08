#pragma once

#include <vector>

class Particle;

class ParticleManager
{
public:
    static void Create();
    static void Destroy();
    static bool IsValid();

    static ParticleManager& Instance(); 


    // Any specific funcs here
    void LoadParticleDefinitions();
    void Update();

protected:
    static ParticleManager* sInstance;


private:
    ParticleManager() = default;
   ~ParticleManager() = default;

   //TODO - private...
public:
    std::vector<Particle> mParticles;

   // Effective Modern C++ book: that deleted functions should generally be public as it results in better error messages
   // due to the compilers behavior to check accessibility before deleted status
public:
    ParticleManager(const ParticleManager&) = delete; //TODO - do I do these decl wrong?
    void operator=(const ParticleManager&) = delete;

};