#include "Particle.h"

ParticleDefinition::ParticleDefinition(const nlohmann::json json)
{
//     mPosition.x = json["mPosition"]["x"];
//     mPosition.y = json["mPosition"]["y"];
//     mPosition.z = json["mPosition"]["z"];
    mLifeTime = json["mLifeTime"];
    mTextures[0] = json["mTextures"][0];
    mTextures[1] = json["mTextures"][1];
    mTextures[2] = json["mTextures"][2];
    mTextures[3] = json["mTextures"][3];
}