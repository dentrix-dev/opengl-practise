#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <model.h>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// -------------- Vertex ---------------
Vertex::Vertex(glm::vec3 Position) {
    this->Position = Position;
}

// -------------- Mesh ----------------
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices = indices;

    setup();
}

void Mesh::setup() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glBindVertexArray(0);
}

void Mesh::Draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// ------------------- Model ----------------
Model::Model(std::string path) {
    loadModel(path);
}

void Model::Draw() {
    for (int i=0; i<meshes.size(); i++) {
        meshes[i].Draw();
    }
}

void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene) {
    for (int i=0; i<node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (int i=0; i<node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}


Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (int i=0; i<mesh->mNumVertices; i++) {
        glm::vec3 v(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertices.push_back(Vertex(v));
    }

    for (int i=0; i<mesh->mNumFaces; i++) {
        indices.push_back(mesh->mFaces[i].mIndices[0]);
        indices.push_back(mesh->mFaces[i].mIndices[1]);
        indices.push_back(mesh->mFaces[i].mIndices[2]);
    }

    return Mesh(vertices, indices);
}










