#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct Particle
{
    glm::vec3 position;
    glm::vec3 velocity;

    float life;
    float maxLife;
    float size;
};

struct ParticleInstance
{
    float x;
    float y;
    float z;
    float size;
    float alpha;
};

class ParticleSystem
{
public:
    ParticleSystem();

    void Init(unsigned int maxParticles);
    void Update(float deltaTime);
    void Render(const glm::mat4& view, const glm::mat4& projection);

    void Emit(const glm::vec3& position);

    ~ParticleSystem();

private:
    std::vector<Particle> particles;

    GLuint VAO = 0;

    GLuint quadVBO = 0;


    GLuint instanceVBO = 0;

    unsigned int maxParticles = 0;

    void RespawnParticle(Particle& particle, const glm::vec3& position);
    void BuildInstanceData(std::vector<ParticleInstance>& instances);
};