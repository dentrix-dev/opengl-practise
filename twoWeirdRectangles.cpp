#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//vertex shader
const char *vertexShaderSource = "#version 330 core\n"//glsl version 3.3
                                 "layout (location = 0) in vec3 aPos;\n"//define attrib aPos for each vertex
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"//converts i/p 3d vector to homogenous vec4
                                 "}\0";

//fragment (color) shader
const char *fragmentShaderSource =  "#version 330 core\n"
                                    "out vec4 FragColor;\n"//define output variable for fragment/pixel color
                                    "void main()\n"
                                    "{\n"
                                    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"//every fragment will be this color since there is no texture mapping
                                    "}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    std::cout<<"resize callback cdSalled"<<std::endl;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void checkVertexShaderCompilationStatus(unsigned int vertexShader){
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout<<"shader::vertex::compilation_failed\n"<<infoLog<<std::endl;
    }
}

void checkFragmentShaderCompilationStatus(unsigned int fragmentShader){
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout<<"shader::fragment::compilation_failed\n"<<infoLog<<std::endl;
    }
}

void checkLinkingShaderProgramStatus(unsigned int shaderProgram){
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout<<"shader::program::linking_failed\n"<<infoLog<<std::endl;
    }
}

int main()
{
    //init glfw and configure
    //-----------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw window creation
    //--------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //register callback for window resize -- in general as is usual, register the callbacks of the window before the render loop
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad: load all opengl functions pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //build and compile our shader program
    //------------------------------------
    //create vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //set shader source
    //(shader object, num of strings of shader source, shader source code, leave it for now)
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //compile
    glCompileShader(vertexShader);
    checkVertexShaderCompilationStatus(vertexShader);

    //create fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkFragmentShaderCompilationStatus(fragmentShader);

    //link both shaders to a shader program to be used
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);//vertex shader first in program shader pipeline
    glAttachShader(shaderProgram, fragmentShader);//fragement shader second in program shader pipeline
    glLinkProgram(shaderProgram);
    checkLinkingShaderProgramStatus(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    //input vertex data - note that since we want a 2d triangle for each point z is zero
    float vertices[] = {
        -0.5f,  0.5f, 0.0f,  // top right
        -0.5f, -0.5f, 0.0f,  // bottom right
        -0.9f, -0.5f, 0.0f,  // bottom left
        -0.9f,  0.5f, 0.0f,   // top left
        ///second triangle
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        0.2f, -0.5f, 0.0f,  // bottom left
        0.2f,  0.5f, 0.0f,   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3,    // second triangle

        4, 5, 7,    //first triangle
        5, 6, 7     //second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    //creat the vertex and element buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first so it becomes active and subsequent buffer settings saved
    glBindVertexArray(VAO);

    //bind and upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //bind and upload element data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //attribute index (location 0), 3 vals per vertex (x,y,z), float, no normalization, stride (distance between consecutive vertices), starting pos in buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);//enable the attribute at location 0

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        //input
        //----
        processInput(window);

        //render
        //------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//state setting function
        glClear(GL_COLOR_BUFFER_BIT);//state-using function, uses curr state to retrieve clearing color from

        //draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(indices[0]), GL_UNSIGNED_INT, 0);//(type, num elements, element data type, start index)
        // glBindVertexArray(0); // no need to unbind it every time

        //swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
