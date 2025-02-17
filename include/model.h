#include "glm/ext/vector_float3.hpp"
#include "shader.h"
#include <assimp/scene.h>
#include <vector>

struct Vertex {
    glm::vec3 Position;
  glm::vec3 Normal;

    Vertex(glm::vec3 Position, glm::vec3 Normal);
};

class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

    void Draw();

  private:
    unsigned VAO, VBO, EBO;

    void setup();
};

class Model {
  public:
    Model(std::string path);
    void Draw();

  private:
    // model data
    std::vector<Mesh> meshes;

    void loadModel(std::string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};
