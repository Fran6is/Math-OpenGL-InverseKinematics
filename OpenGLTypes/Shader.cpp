#include <glad/glad.h>
#include "Shader.h"
#include <fstream>
#include <string>

#define TOSTRING(X) (#X)

Shader::Shader(const char* VertexShaderFilePath, const char* FragmentShaderFilePath)
{
    std::string VertexShaderCode{};
    std::string FragmentShaderCode{};

    bool VertexLoadingWasSuccessful   = LoadShaderFromFile(VertexShaderFilePath, VertexShaderCode);
    bool FragmentLoadingWasSuccessful = LoadShaderFromFile(FragmentShaderFilePath, FragmentShaderCode);

    if (VertexLoadingWasSuccessful && FragmentLoadingWasSuccessful)
    {
        ProgramID = CompileShaderCode(VertexShaderCode, FragmentShaderCode);
    }
}

void Shader::Use() const
{
    glUseProgram(ProgramID);
}

bool Shader::LoadShaderFromFile(const char* ShaderFilePath, std::string& OutShaderCode)
{
    std::ifstream ShaderFile{ ShaderFilePath };
    if (!ShaderFile.is_open())
    {
        std::cout << "Unable to load shader file at path: " << ShaderFilePath << "\n";
        return false;
    }
    std::string temp{};

    while (std::getline(ShaderFile, temp))
        OutShaderCode += temp + "\n";

    ShaderFile.close();
    return true;
}

unsigned int Shader::CompileShaderCode(const std::string& VertexShaderCode, const std::string& FragmentShaderCode)
{
    const char* shaderSource_C_str = VertexShaderCode.c_str();

    // Vertex Shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &shaderSource_C_str, NULL);
    glCompileShader(vertexShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Fragment Shader
    shaderSource_C_str = FragmentShaderCode.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &shaderSource_C_str, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    if(!success) __debugbreak();

    // Create and link to a program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void Shader::SetBool(const std::string& VariableName, bool Value) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());
    if (location != -1)
    {
        glUniform1i(location, static_cast<int>(Value));
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetBool()), __LINE__);  //__debugbreak();
    }
}

void Shader::SetInt(const std::string& VariableName, int Value) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());

    if (location != -1)
    {
        glUniform1i(location, Value);
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetInt()), __LINE__); //__debugbreak();
    }
}

void Shader::SetFloat(const std::string& VariableName, float Value) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());
    if (location != -1)
    {
        glUniform1f(location, Value);
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetFloat()), __LINE__); //__debugbreak();
    }
}

void Shader::SetVector4(const std::string& VariableName, float Vector4[4]) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());
    if (location != -1)
    {
        glUniform4f(location, Vector4[0], Vector4[1], Vector4[2], Vector4[3]);
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetVector4), __LINE__);  //__debugbreak();
    }
}

void Shader::SetVector3(const std::string& VariableName,  const glm::vec3& Vector) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());
    if (location != -1)
    {
        glUniform3f(location, Vector.x, Vector.y, Vector.z);
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetVector2), __LINE__);  //__debugbreak();
    }
}

void Shader::SetVector2(const std::string& VariableName, float Vector2[2]) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());
    if (location != -1)
    {
        glUniform2f(location, Vector2[0], Vector2[1]);
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetVector2), __LINE__);  //__debugbreak();
    }
}

void Shader::SetVector2(const std::string& VariableName, const glm::vec2& Vec) const
{
    float vec[] = { Vec.x, Vec.y };
    SetVector2(VariableName, vec);
}

void Shader::SetMat3(const std::string& VariableName, const glm::mat3& Matrix) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());
    if (location != -1)
    {
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(Matrix));
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetMat4), __LINE__);  //__debugbreak();
    }
}

void Shader::SetMat4(const std::string& VariableName, const glm::mat4& Matrix) const
{
    int location = glGetUniformLocation(ProgramID, VariableName.c_str());
    if (location != -1)
    {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(Matrix));
    }
    else
    {
        ReportInvalidUniformName(VariableName.c_str(), TOSTRING(SetMat4), __LINE__);  //__debugbreak();
    }
}


void Shader::ReportInvalidUniformName(const char* UniformName, const char* FunctionName, int LINE) const
{
    std::cerr << "Log: No Uniform variable with name '" << UniformName << "' found in your shader program. Uniforms can also be removed from program if not used. ::"
        << FunctionName << ", line " << LINE << std::endl;
}
