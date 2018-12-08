#include "Particle.h"
#include "ParticleManager.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

// for convenience
using json = nlohmann::json;

ParticleManager* ParticleManager::sInstance = nullptr;

/*static*/ void ParticleManager::Create()
{
    if (!sInstance)
    {
        sInstance = new ParticleManager();
    }
}

/*static*/ void ParticleManager::Destroy()
{
    if (sInstance)
    {
        delete sInstance;
        sInstance = nullptr;
    }
}

/*static*/ bool ParticleManager::IsValid()
{
    return sInstance != nullptr;
}

/*static*/ ParticleManager& ParticleManager::Instance()
{
    //assert if sInstance is null? TODO
    return *sInstance;
}

void ParticleManager::LoadParticleDefinitions()
{
    // Load a bunch of JSON files...
    
    /*
    {
        "mPosition" : {"x":0, "y":0,"z":0},
        "mLifeTime" : 10,
        "mEndDisplayTime" : 0,
        "mTextures" : [
            0,
            0,
            0,
            0
        ]
    }
    */

    // read a JSON file
    std::ifstream i("../DATA/Triangle1.json");
    json j;
    i >> j;
    /*//
    /std::ifstream t("../DATA/Triangle1.json");
    std::string contents((std::istreambuf_iterator<char>(t)),
                          std::istreambuf_iterator<char>());

    std::cout << contents << std::endl;

    // serialization with pretty printing
    // pass in the amount of spaces to indent
    std::cout << j.dump(4) << std::endl;

    // create object from string literal
    json j = contents.c_str()_json;*/
    std::cout << j << std::endl;
    std::cout << j.dump(4) << std::endl;

    Particle p;
    p.mPosition.x = j["mPosition"]["x"];
    p.mPosition.y = j["mPosition"]["y"];
    p.mPosition.z = j["mPosition"]["z"];
    p.mLifeTime = j["mLifeTime"];
    p.mTextures[0] = j["mTextures"][0];
    p.mTextures[1] = j["mTextures"][1];
    p.mTextures[2] = j["mTextures"][2];
    p.mTextures[3] = j["mTextures"][3];
    mParticles.push_back(p); //TODO improve so no copy
    return;
    //people_file >> people;



}

void ParticleManager::Update()
{
    static int cubeNumber = 0;
    static std::chrono::system_clock::time_point nextObj = std::chrono::system_clock::now();// +std::chrono::seconds(2);
                                                                                           // world space positions of our cubes
    static glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    mParticles.erase(std::remove_if(
        mParticles.begin(), mParticles.end(),
        [](const Particle& x) {
        return x.mEndDisplayTime < std::chrono::system_clock::now(); // put your condition here
    }), mParticles.end());


    if (nextObj < std::chrono::system_clock::now())
    {
        nextObj = std::chrono::system_clock::now() + std::chrono::seconds(1);
        Particle p;
        //p.mLifeTime = 10;
        //float angle = 20.0f * i;
        //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        p.mPosition = cubePositions[cubeNumber++];
        //temp hack so I can always see the triangles on top of the cubes
        p.mPosition.z += 1;
        if (cubeNumber > 9)
        {
            cubeNumber = 0;
        }
        //std::chrono::system_clock::duration lifeTime;
        //lifeTime.
        p.mEndDisplayTime = std::chrono::system_clock::now() + std::chrono::seconds(p.mLifeTime);


        if ((cubeNumber % 2) == 1)
        {
            //uhhh damn, I need to do this for loadded particles. //TODO
            p.mTextures[0] = 1;
            p.mTextures[1] = 2;
            //p.mTextures[0] = texture1;
           // p.mTextures[1] = texture2;
        }
        else
        {
            p.mTextures[0] = 2;
            p.mTextures[1] = 1;
            //p.mTextures[0] = texture1;
            //p.mTextures[1] = texture2;
        }


        mParticles.push_back(p);
    }


}