#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <stdio.h>
//#include "shader_s.h"
#include <soil.h>


struct Object {
    unsigned int VAO, VBO[2], EBO;
    float* vertices;
    float* texCoords;
    unsigned int* indices;
    int numVertices, numIndices;
    unsigned int texture;
};

void createObject(Object& obj, float* vert, float* tex, unsigned int* ind, int nV, int nI, const char* texturePath) {
    // generate buffers
    glGenVertexArrays(1, &obj.VAO);
    glGenBuffers(2, obj.VBO);
    glGenBuffers(1, &obj.EBO);

    // vao
    glBindVertexArray(obj.VAO);

    // vertex data
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, nV * 3 * sizeof(float), vert, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    // texCoords
    glBindBuffer(GL_ARRAY_BUFFER, obj.VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, nV * 2 * sizeof(float), tex, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nI * sizeof(unsigned int), ind, GL_STATIC_DRAW);

    // texture loading
    glGenTextures(1, &obj.texture);
    glBindTexture(GL_TEXTURE_2D, obj.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height;
    unsigned char* image = SOIL_load_image(texturePath, &width, &height, 0, SOIL_LOAD_RGB);
    if (image) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

}


int main() {
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW \n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "My Plane", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    // road
    float roadVertices[] = {
        //крыло левое
        -0.0f, 0.1f, 0.0f,
        0.4f, 0.1f, 0.0f,
        0.2f, 0.5f, 0.0f,
    };

    float roadTexCoords[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    unsigned int roadIndices[] = { 0, 1, 2};

    // basis
    float basisVertices[] = {
        //крыло правое 
        0.35f, -0.1f, 0.0f,
        0.15f, -0.6f, 0.0f,
        0.0f, -0.1f, 0.0f,

    };

    float basisTexCoords[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f
    };

    unsigned int basisIndices[] = { 0, 1, 2 };

    float tailVertices[] = {
        //хвост, к нему идёт одна точка прямоугольника - левая верхняя
        -0.6f, 0.3f, 0.0f,
        -0.5f, 0.1f ,0.0f,
        - 0.6f, 0.1f, 0.0f
    };

    float tailTexCoords[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    unsigned int tailIndices[] = { 0, 1, 2};

    // голова
    float truckVertices[] = {
        0.8f, -0.1f, 0.0f,
         0.6f, 0.1f, 0.0f, // к голове
         0.6f, -0.1f, 0.0f, // к голове
    };

    float truckTexCoords[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    unsigned int truckIndices[] = { 0, 1, 2 };

    // cabin
    float cabinVertices[] = {
         -0.6f, 0.1f, 0.0f, //tail
         0.6f, 0.1f, 0.0f, // к голове
         0.6f, -0.1f, 0.0f, // к голове
        -0.6f, -0.1f, 0.0f,
    };

    float cabinTexCoords[] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    unsigned int cabinIndices[] = { 0, 1, 3, 1, 2, 3 };


    Object road, basis, cabin, truck, tail;

    createObject(road, roadVertices, roadTexCoords, roadIndices,
        4, 6, "textures/winds.jpg");
    createObject(basis, basisVertices, basisTexCoords, basisIndices,
        4, 6, "textures/winds.jpg");
    createObject(tail, tailVertices, tailTexCoords, tailIndices,
        4, 6, "textures/texture.jpg");
    createObject(truck, truckVertices, truckTexCoords, truckIndices,
        4, 6, "textures/cabin_texture.jpg");
    createObject(cabin, cabinVertices, cabinTexCoords, cabinIndices,
        4, 6, "textures/cabin_texture.jpg");
    

    const char* vertexShaderSource =
        "#version 400\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec2 aTexCoord;\n"

        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "   TexCoord = aTexCoord;\n"
        "}\0";

    const char* fragmentShaderSource =
        "#version 400\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D texture;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(texture, TexCoord);\n"
        "}\n\0";

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // rendering
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.5f, 0.7f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindTexture(GL_TEXTURE_2D, road.texture);
        glBindVertexArray(road.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, basis.texture);
        glBindVertexArray(basis.VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, tail.texture);
        glBindVertexArray(tail.VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


        glBindTexture(GL_TEXTURE_2D, truck.texture);
        glBindVertexArray(truck.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D, cabin.texture);
        glBindVertexArray(cabin.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glfwTerminate();
    return 0;
}