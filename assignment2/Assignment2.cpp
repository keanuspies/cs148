#include "assignment2/Assignment2.h"
#include "common/core.h" // <-- haha.
#include "common/Utility/Mesh/Simple/PrimitiveCreator.h"
#include "common/Utility/Mesh/Loading/MeshLoader.h"
#include <cmath>
#include <fstream>
#include <string>

namespace
{
const int SHADER_ERROR_LOG_SIZE = 500;
bool VerifyShaderCompile(GLuint newShaderObject)
{
    GLint compileStatus;
    OGL_CALL(glGetShaderiv(newShaderObject, GL_COMPILE_STATUS, &compileStatus));
    if (compileStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetShaderInfoLog(newShaderObject, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Shader compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}

bool VerifyProgramLink(GLuint shaderProgram)
{
    GLint linkStatus;
    OGL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus));
    if (linkStatus != GL_TRUE) {
        char errorLogBuffer[SHADER_ERROR_LOG_SIZE];
        OGL_CALL(glGetProgramInfoLog(shaderProgram, SHADER_ERROR_LOG_SIZE, NULL, errorLogBuffer));
        std::cerr << "ERROR: Program link compilation failure -- " << std::endl << errorLogBuffer << std::endl;
        return false;
    }
    return true;
}
}

Assignment2::Assignment2(std::shared_ptr<class Scene> inputScene, std::shared_ptr<class Camera> inputCamera):
    Application(std::move(inputScene), std::move(inputCamera))
{
    vertexPositions = 
        std::initializer_list<glm::vec4>({
            // Triangle 1
            {0.f, 0.f, 0.f, 1.f},
            {1.f, 0.f, 0.f, 1.f},
            {1.f, 1.f, 0.f, 1.f},
            // Triangle 2
            {0.f, 0.f, 0.f, 1.f},
            {-1.f, 1.f, 0.f, 1.f},
            {-1.f, 0.f, 0.f, 1.f}
        });
    time = 0.0;
}

std::unique_ptr<Application> Assignment2::CreateApplication(std::shared_ptr<class Scene> scene, std::shared_ptr<class Camera> camera)
{
    return make_unique<Assignment2>(std::move(scene), std::move(camera));
}

std::shared_ptr<class Camera> Assignment2::CreateCamera()
{
    // Specify any old aspect ratio for now, we'll update it later once the window gets made!
    // Read more about Field of View: http://rg3.name/201210281829.html!
    // Note that our field of view is the VERTICAL field of view (in degrees).
    return std::make_shared<PerspectiveCamera>(75.f, 1280.f / 720.f);
}

glm::vec2 Assignment2::GetWindowSize() const
{
    return glm::vec2(1280.f, 720.f);
}

void Assignment2::SetupScene()
{
    SetupExample1();
}

void Assignment2::SetupCamera()
{
}

void Assignment2::HandleInput(SDL_Keysym key, Uint32 state, Uint8 repeat, double timestamp, double deltaTime)
{   
    Application::HandleInput(key, state, repeat, timestamp, deltaTime);
}

void Assignment2::HandleWindowResize(float x, float y)
{
    Application::HandleWindowResize(x, y);
}

void Assignment2::SetupExample1()
{
    // Insert "Load and Compile Shaders" code here.
    const std::string vertexFilename = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.vert";
    std::ifstream inFile(vertexFilename);
    std::string vertexData((std::istreambuf_iterator<char>(inFile)),
                 std::istreambuf_iterator<char>());
    const std::string fragmentFilename = std::string(STRINGIFY(SHADER_PATH)) + "/hw2/hw2.frag";
    std::ifstream inFile2(fragmentFilename);
    std::string fragmentData((std::istreambuf_iterator<char>(inFile2)),
                 std::istreambuf_iterator<char>());

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

    const char *vertex_cstr = vertexData.c_str();
    const char *fragment_cstr = fragmentData.c_str();

    glShaderSource(vertexShaderId, 1, &vertex_cstr, NULL);
    glShaderSource(fragmentShaderId, 1, &fragment_cstr, NULL);

    glCompileShader(vertexShaderId);
    glCompileShader(fragmentShaderId);

    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShaderId);
    glAttachShader(shaderProgramId, fragmentShaderId);

    glLinkProgram(shaderProgramId);

    // Checkpoint 1.
    // Modify this part to contain your vertex shader ID, fragment shader ID, and shader program ID.

    // DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION
    if (!VerifyShaderCompile(vertexShaderId) || !VerifyShaderCompile(fragmentShaderId) || !VerifyProgramLink(shaderProgramId)) {
        std::cout << "FAILURE: Checkpoint 1 failed." << std::endl;
    } else {
        std::cout << "SUCCESS: Checkpoint 1 completed." << std::endl;
    }

    OGL_CALL(glDetachShader(shaderProgramId, vertexShaderId));
    OGL_CALL(glDeleteShader(vertexShaderId));
    OGL_CALL(glDetachShader(shaderProgramId, fragmentShaderId));
    OGL_CALL(glDeleteShader(fragmentShaderId));
    // FINISH DO NOT EDIT OR REMOVE THE CODE IN THIS SECTION

    // Insert "Setup Buffers" code here.
    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions)*sizeof(float), &vertexPositions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Assignment2::Tick(double deltaTime)
{
    // Insert "Send Buffers to the GPU" and "Slightly-More Advanced Shaders" code here.
    glUseProgram(shaderProgramId);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    time += deltaTime;
    
    GLint location = glGetUniformLocation(shaderProgramId, "inputTime");
    glUniform1f(location, time);

}
