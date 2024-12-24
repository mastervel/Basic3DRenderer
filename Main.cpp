

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_m.h"
#include "window.h"
#include "model.h"

#include <iostream>

void processInput(const Window& appWindow);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    Window window(SCR_WIDTH, SCR_HEIGHT, "FPS Game");


    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // Point Light Positions (Lamb Cubes)
    // -----------------------------
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    // build and compile shaders
    // -------------------------
    Shader modelShader("vs.model", "fs.model");
    Shader lambShader("vs.lamb", "fs.lamb");

    // load models
    // -----------
    //Model ourModel("E:/OpenGLProject/backpack/backpack.obj");
    Model ourModel("E:/OpenGLProject/objects/male.obj");

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // lamb cubes
    float lightVertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };

    unsigned int lightCubeVAO, lightCubeVBO;
    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &lightCubeVBO);

    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    float modelSpeed = 2.0f; // Speed of the model movement
    glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f); // Example model position

    // render loop
    // -----------
    while (!window.ShouldClose())
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.6f, 0.6f, 0.6f, 0.6f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        modelShader.use();
        modelShader.setVec3("viewPos", camera.Position);
        modelShader.setFloat("shininess", 64.0f);

        //lighting parameters
        // directional light
        modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        modelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        modelShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        modelShader.setVec3("pointLights[0].ambient", 0.15f, 0.15f, 0.15f);
        modelShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[0].constant", 1.0f);
        modelShader.setFloat("pointLights[0].linear", 0.09f);
        modelShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        modelShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        modelShader.setVec3("pointLights[1].ambient", 0.15f, 0.15f, 0.15f);
        modelShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[1].constant", 1.0f);
        modelShader.setFloat("pointLights[1].linear", 0.09f);
        modelShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        modelShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        modelShader.setVec3("pointLights[2].ambient", 0.15f, 0.15f, 0.15f);
        modelShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[2].constant", 1.0f);
        modelShader.setFloat("pointLights[2].linear", 0.09f);
        modelShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        modelShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        modelShader.setVec3("pointLights[3].ambient", 0.15f, 0.15f, 0.15f);
        modelShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights[3].constant", 1.0f);
        modelShader.setFloat("pointLights[3].linear", 0.09f);
        modelShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        modelShader.setVec3("spotLight.position", camera.Position);
        modelShader.setVec3("spotLight.direction", camera.Front);
        modelShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        modelShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("spotLight.constant", 1.0f);
        modelShader.setFloat("spotLight.linear", 0.09f);
        modelShader.setFloat("spotLight.quadratic", 0.032f);
        modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        
        glm::vec3 cameraPosition = camera.GetCameraPosition();

        // Project the direction vector onto the X-Z plane
        glm::vec3 direction = glm::normalize(glm::vec3(cameraPosition.x - modelPosition.x, 0.0f, cameraPosition.z - modelPosition.z));
        modelPosition += direction * modelSpeed * deltaTime;

        // Define the up vector for X-Z rotation
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        // Calculate the right vector
        glm::vec3 right = glm::normalize(glm::cross(up, direction));

        // Create the rotation matrix
        glm::mat4 rotationMatrix = glm::mat4(
            glm::vec4(right, 0.0f),
            glm::vec4(up, 0.0f),
            glm::vec4(direction, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
        );

        // Apply rotation and translation to the model matrix
        model = glm::translate(model, modelPosition); // translate it down so it's at the center of the scene
        model *= rotationMatrix;
        model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down

        modelShader.setMat4("model", model);
        modelShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
        ourModel.Draw(modelShader, false);

        // also draw the lamp object(s)
        lambShader.use();
        lambShader.setMat4("projection", projection);
        lambShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lambShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        window.Update();
    }
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(const Window& appWindow)
{
    // Get the GLFWwindow pointer from the Window class
    GLFWwindow* window = appWindow.GetWindow();

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}