#include "ParticleSystem.h"

#include <cstdlib>
#include <ctime>
#include <cstddef>

ParticleSystem::ParticleSystem()
{
    std::srand((unsigned int)std::time(nullptr));
}

void ParticleSystem::Init(unsigned int maxParticles)
{
    this->maxParticles = maxParticles;
    particles.resize(maxParticles);

    for (unsigned int i = 0; i < maxParticles; i++)
    {
        particles[i].life = 0.0f;
        particles[i].maxLife = 1.0f;
        particles[i].size = 0.2f;
        particles[i].position = glm::vec3(0.0f);
        particles[i].velocity = glm::vec3(0.0f);
    }

    float quadVertices[] =
    {
        // localPos      // UV
        -0.5f, -0.5f,    0.0f, 0.0f,
         0.5f, -0.5f,    1.0f, 0.0f,
         0.5f,  0.5f,    1.0f, 1.0f,

        -0.5f, -0.5f,    0.0f, 0.0f,
         0.5f,  0.5f,    1.0f, 1.0f,
        -0.5f,  0.5f,    0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    // VBO 1: quad base

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(quadVertices),
        quadVertices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)0
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
        1,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // VBO2: datos de instancia
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        maxParticles * sizeof(ParticleInstance),
        nullptr,
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        2,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ParticleInstance),
        (void*)offsetof(ParticleInstance, x)
    );
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);

    glVertexAttribPointer(
        3,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ParticleInstance),
        (void*)offsetof(ParticleInstance, size)
    );
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);

    glVertexAttribPointer(
        4,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(ParticleInstance),
        (void*)offsetof(ParticleInstance, alpha)
    );
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ParticleSystem::Emit(const glm::vec3& position)
{
    for (unsigned int i = 0; i < particles.size(); i++)
    {
        if (particles[i].life <= 0.0f)
        {
            RespawnParticle(particles[i], position);
            return;
        }
    }
}

void ParticleSystem::RespawnParticle(Particle& particle, const glm::vec3& position)
{
    float randomX = ((std::rand() % 100) / 100.0f) - 0.5f;
    float randomZ = ((std::rand() % 100) / 100.0f) - 0.5f;

    particle.position = position;

    particle.velocity = glm::vec3(
        randomX * 0.7f,
        1.5f + ((std::rand() % 100) / 100.0f),
        randomZ * 0.7f
    );

    particle.life = 1.0f;
    particle.maxLife = 1.0f;

    particle.size = 0.15f + ((std::rand() % 100) / 100.0f) * 0.25f;
}

void ParticleSystem::Update(float deltaTime)
{
    for (unsigned int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];

        if (p.life > 0.0f)
        {
            p.life -= deltaTime;

            if (p.life > 0.0f)
            {
                p.position += p.velocity * deltaTime;
                p.velocity.y -= 0.7f * deltaTime;
            }
        }
    }
}

void ParticleSystem::BuildInstanceData(std::vector<ParticleInstance>& instances)
{
    instances.clear();
    instances.reserve(maxParticles);

    for (unsigned int i = 0; i < particles.size(); i++)
    {
        Particle& p = particles[i];

        if (p.life <= 0.0f)
            continue;

        ParticleInstance instance;

        instance.x = p.position.x;
        instance.y = p.position.y;
        instance.z = p.position.z;

        instance.size = p.size;
        instance.alpha = p.life / p.maxLife;

        instances.push_back(instance);
    }
}

void ParticleSystem::Render(const glm::mat4& view, const glm::mat4& projection)
{
    (void)view;
    (void)projection;

    std::vector<ParticleInstance> instances;
    BuildInstanceData(instances);

    if (instances.empty())
        return;

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        instances.size() * sizeof(ParticleInstance),
        instances.data()
    );

    glDrawArraysInstanced(
        GL_TRIANGLES,
        0,
        6,
        (GLsizei)instances.size()
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{
    if (instanceVBO != 0)
    {
        glDeleteBuffers(1, &instanceVBO);
        instanceVBO = 0;
    }

    if (quadVBO != 0)
    {
        glDeleteBuffers(1, &quadVBO);
        quadVBO = 0;
    }

    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
}