#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"  // Include stb_image header

// Vertex data for a cube with texture coordinates
float vertices[] = {
    // Positions         // Texture Coords
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Top face
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    // Left face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    // Right face
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
};

// Function prototypes
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(const char* path);
void randomColor(float &r, float &g, float &b); // Function to generate a random color

// Global variables
float angleX = 0.0f;
float angleY = 0.0f;
bool useTexture = false; // Start with textures disable
bool autoRotate = false;
float autoRotateSpeed = 1.0f; // Degrees per scroll unit

// Texture paths for each face
const char* texturePaths[6] = {
    "texture1.jpg", // Back
    "texture2.jpg", // Front
    "texture3.jpg", // Top
    "texture4.jpg", // Bottom
    "texture5.jpg", // Left
    "texture6.jpg"  // Right
};

// Colors for each face
float colors[6][3] = {
    {1.0f, 0.0f, 0.0f}, // Back (red)
    {0.0f, 1.0f, 0.0f}, // Front (green)
    {0.0f, 0.0f, 1.0f}, // Top (blue)
    {1.0f, 1.0f, 0.0f}, // Bottom (yellow)
    {1.0f, 0.0f, 1.0f}, // Left (magenta)
    {0.0f, 1.0f, 1.0f}  // Right (cyan)
};

int main() {
    // Seed random number generator
    std::srand(static_cast<unsigned>(std::time(0)));

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Cube with Textures and Colors", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D

    // Initialize GLEW
    glewInit();

    // Vertex Array Object (VAO) and Vertex Buffer Object (VBO)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Create shaders
    const char* vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec2 texCoord;
        
        out vec2 fragTexCoord;
        uniform mat4 model;
        
        void main() {
            gl_Position = model * vec4(position, 1.0);
            fragTexCoord = texCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 color;
        
        in vec2 fragTexCoord;
        uniform sampler2D texture1;
        uniform bool useTexture;
        uniform vec4 faceColor;

        void main() {
            if (useTexture) {
                color = texture(texture1, fragTexCoord);
            } else {
                // Assign random color (this will be handled in C++)
                color = faceColor;
                // color = vec4(1.0, 0.0, 0.0, 1.0); // Default color (red)
            }
        }
    )";

    // Compile and link shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Cleanup
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Load textures for all 6 faces
    unsigned int textures[6];
    glGenTextures(6, textures);
    for (int i = 0; i < 6; i++) {
        textures[i] = loadTexture(texturePaths[i]);
    }

    // Set the callbacks
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader program
        glUseProgram(shaderProgram);

        // Create model matrix for rotation
        glm::mat4 model = glm::mat4(1.0f);

        // Rotate based on user input
        if (autoRotate) {
            angleY += autoRotateSpeed; // Auto-rotate around Y axis
        }
        model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X
        model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y

        // Pass the model matrix to the shader
        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        // Draw the cube with different textures for each face
        for (int i = 0; i < 6; i++) {
            glBindTexture(GL_TEXTURE_2D, textures[i]);
            glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
            glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), useTexture);

            if (!useTexture) {
                glUniform4f(glGetUniformLocation(shaderProgram, "faceColor"),
                colors[i][0], colors[i][1], colors[i][2], 1.0f);
            }

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, i * 6, 6); // Each face consists of 6 vertices
        }

        // Unbind VAO
        glBindVertexArray(0);

        // Swap front and back buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteTextures(6, textures);
    glfwTerminate();
    return 0;
}

// Key callback function
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_LEFT) {
            angleY -= 5.0f; // Rotate left
        }
        if (key == GLFW_KEY_RIGHT) {
            angleY += 5.0f; // Rotate right
        }
        if (key == GLFW_KEY_UP) {
            angleX -= 5.0f; // Rotate up
        }
        if (key == GLFW_KEY_DOWN) {
            angleX += 5.0f; // Rotate down;
        }
        if (key >= GLFW_KEY_1 && key <= GLFW_KEY_6) {
            int faceIndex = key - GLFW_KEY_1; // Convert key to face index (0-5)
            randomColor(colors[faceIndex][0], colors[faceIndex][1], colors[faceIndex][2]); // Change color
        }
        if (key == GLFW_KEY_T) {
            useTexture = !useTexture; // Toggle texture mode
        }
		if (key == GLFW_KEY_A) {
            autoRotate = !autoRotate; // Toggle auto-rotation
        }
    }
}

// Scroll callback function
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Adjust auto-rotation speed based on scroll
    if (yoffset > 0) {
        autoRotateSpeed += 0.5f; // Increase speed
    } else {
        autoRotateSpeed = std::max(0.0f, autoRotateSpeed - 0.5f); // Decrease speed, but not below zero
    }
}

// Load texture from file
unsigned int loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);
    return textureID;
}

// Function to generate a random color
void randomColor(float &r, float &g, float &b) {
    r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    g = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    b = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}
