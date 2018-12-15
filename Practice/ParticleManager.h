#pragma once

#include <vector>

class Particle;
// for convenience
using json = nlohmann::json;

// ParticleManager
// Responsible for
//  - loading from disk
//  - keeping track of list of particles (update / add / delete, etc...)
//  - 
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

   //void ParseParticleJson(const json& j, Particle& outParticle) const;

   //TODO - private...
public:
    std::vector<Particle> mParticles;

   // Effective Modern C++ book: that deleted functions should generally be public 
   // as it results in better error messages due to the compilers behavior to 
    // check accessibility before deleted status
public:
    //TODO - do I do these decl wrong?
    ParticleManager(const ParticleManager&) = delete; 
    void operator=(const ParticleManager&) = delete;

};