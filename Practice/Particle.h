#pragma once

#include <chrono>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>


// what is a particle?
// texture
// lifetime


// Could forsee this getting more complex, like having particle definitions and particle instances, having some sort of factory to make them
// and that factory will handle memory management and stuff...

// emitters?

class ParticleDefinition
{
public:
    ParticleDefinition() = default;
    ~ParticleDefinition() = default;

    ParticleDefinition(const nlohmann::json json);

    //glm::vec3 mPosition;// = glm::vec3::ZERO();
    int mLifeTime = 10;
    //int mDurationSeconds = 10;
    //std::chrono::system_clock::time_point mEndDisplayTime;

    //vector of texture IDs (assuming the index = the texture unit being used)
    unsigned int mTextures[4];

};


class ParticleInstance
{
public:
    ParticleInstance() = default;
    ~ParticleInstance() = default;

    //ParticleInstance();

    ParticleDefinition* definition;

    glm::mat4 mModel;
    glm::vec3 mPosition;// = glm::vec3::ZERO();
    
    std::chrono::system_clock::time_point mEndDisplayTime; ///convient? do I need?
};

//What is the best way to lay this out in memory? In terms of a split between ParticleDef / ParticleInst

/*
position
lifetime
textures


I think artists author a particle definition
then we can spawn instances of that particle from code, script, w/e...

lifetime - on def or instance? I think it has to be def, artist does work depending on it


*/
// What does my update loop look like? 
// Loop through each particle and update the mModel field based on w/e is in the particle def (like rotation by time)

// ParticleDef1
//   ParticleInst1
//   ParticleInst2
//   ParticleInst3

// How about lifetime? How is that accessed? it'd be cheapest to cleanup as we're iterating thru so we
// only have to do so once

//Given how much particles are going to be added/deleted, a straight up vector is going to be bad
//I want something tight in memory though
// --> linked list within a page?

//So shall I split out the data I need for the update loop to reduce the size? i.e. ParticleRenderInst? ParticleUpdateInst?

/*
class ParticleUpdateManager
{


ParticleDefiniton mParticleDefinition;
LinkedList mParticlesOfThisDefinition;

}

// If I have a lot of different particle defintions, this is gunna suck if I do 1 per page
// ParticleDefinition ~20 bytes
// ParticleUpdateInfo ~84

mat4: 64
vec3: 12
timepoint: 8
pointer to def: 4
//88 size

num of particles in a page ~ 46
*/


//LEFT OFF HERE - question - where does code live in memory? it lives in the text segment or the "code segment".
// Can that get paged in and out of memory???