#include "Mesh.h"

Mesh::Mesh()
{
}

void Mesh::CreateMesh(GLfloat* vertices, GLuint* indices, GLuint numVertices, GLsizei numIndices)
{
    indexCount = numIndices;
    glGenVertexArrays(1, &VAO); //Genera y devuelve id
    glBindVertexArray(VAO); //Activa VAO

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0])*numIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0])*numVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::CreateMeshFromVertices(vertex_t* vertices, GLuint* indices, GLuint numVertices, GLsizei numIndices)
{
    indexCount = numIndices;
    glGenVertexArrays(1, &VAO); //Genera y devuelve id
    glBindVertexArray(VAO); //Activa VAO

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, vPos));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, vNorm));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, vTextCoords));
    glEnableVertexAttribArray(2);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Mesh::DeleteMesh()
{
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
}

void Mesh::RenderMesh()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    DeleteMesh();
}

void Mesh::RecalculateNormals(GLfloat* vertices, GLuint* indices, GLuint numVertices, GLsizei numIndices, unsigned int numDatosV, unsigned int offsetNormals)
{
    for (int i = 0; i < numIndices; i += 3) {
        unsigned int index0 = indices[i] * numDatosV;
        unsigned int index1 = indices[i+1] * numDatosV;
        unsigned int index2 = indices[i+2] * numDatosV;

        glm::vec3 v(vertices[index1] - vertices[index0], vertices[index1 + 1] - vertices[index0 + 1], vertices[index1 + 2] - vertices[index0 + 2]);
        glm::vec3 u(vertices[index2] - vertices[index0], vertices[index2 + 1] - vertices[index0 + 1], vertices[index2 + 2] - vertices[index0 + 2]);
        glm::vec3 normal = glm::normalize(glm::cross(v, u));
        
        vertices[index0 + offsetNormals] += normal.x;
        vertices[index0 + offsetNormals+1] += normal.y;
        vertices[index0 + offsetNormals+2] += normal.z;

        vertices[index1 + offsetNormals] += normal.x;
        vertices[index1 + offsetNormals + 1] += normal.y;
        vertices[index1 + offsetNormals + 2] += normal.z;

        vertices[index2 + offsetNormals] += normal.x;
        vertices[index2 + offsetNormals + 1] += normal.y;
        vertices[index2 + offsetNormals + 2] += normal.z;

    }

    for (int i = 0; i < numVertices / numDatosV; i++) {
        unsigned int vertexNormIndex = i * numDatosV + offsetNormals;
        glm::vec3 normal(vertices[vertexNormIndex], vertices[vertexNormIndex + 1], vertices[vertexNormIndex + 2]);
        normal = glm::normalize(normal);
        vertices[vertexNormIndex] = normal.x;
        vertices[vertexNormIndex + 1] = normal.y;
        vertices[vertexNormIndex + 2] = normal.z;
    }
}
