#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

// OpenGL doesn't have default vertex or fragment shaders
// Vertex shader defined here for now
const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";

// Fragment (color) shader
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                   "}\0";

// Resize viewport when window size changes
void framebuffer_size_callback(GLFWwindow *window, int width, int height) { glViewport(0, 0, width, height); }

// Handle input
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    // --------------------- Initalization ---------------------
    // Initialize GLFW and specify version
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Make a window object and set the current context to it
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Use GLAD to link OS-specific function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Specify openGL viewport size
    glViewport(0, 0, 800, 600);

    // Register window size callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // --------------------- Shaders ---------------------
    // Variables for checking compilation status
    int success;
    char infoLog[512];

    // 1- Vertex shader
    // Create shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER); // Specify the shader type
    // Provide the source code to the shader object and compile it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if shader was compiled correctly
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 2- Fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Now that both shaders are compiled, we need to link them in a shader
    // program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Also check for errors when linking
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Delete shader objects as they are not needed anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // --------------------- Shape setup ---------------------
    // Define vertices
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f, 
        -0.5f, 0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f
    };

    // Define indices
    unsigned int indices[] = {
        0, 1, 2, 
        0, 1, 3
    };

    // Define objects to hold the data and attributes
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // Bind the VAO, and store the following VBO and EBO
    glBindVertexArray(VAO);

    // Bind the buffer and load the vertices data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // This operates on VBO
    // The same for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // We need to specify how the vertex data should be interpreted
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Set background color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader program
        glUseProgram(shaderProgram);
        // Bind VAO and draw the shape
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Render color buffers
        glfwSwapBuffers(window);

        // Poll for events such as keyboard or mice
        glfwPollEvents();
    }

    // Exit cleanly
    glfwTerminate();

    return 0;
}
