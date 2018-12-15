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


class Particle
{
public:
    Particle() = default;
    ~Particle() = default;

    Particle(const nlohmann::json json);

    //glm::mat4 mModel;
    glm::vec3 mPosition;// = glm::vec3::ZERO();
    int mLifeTime = 10;
    //int mDurationSeconds = 10;
    std::chrono::system_clock::time_point mEndDisplayTime;

    //vector of texture IDs (assuming the index = the texture unit being used)
    unsigned int mTextures[4];

};