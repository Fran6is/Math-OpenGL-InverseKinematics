#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    Shader(const char* VertexShaderFilePath, const char* FragmentShaderFilePath);

    void Use() const;
    void SetBool(const std::string& VariableName, bool Value) const;
    void SetInt(const std::string& VariableName, int Value)  const;
    void SetFloat(const std::string& VariableName, float Value) const;
    void SetVector4(const std::string& VariableName, float Vector4AsArray[4]) const;
    void SetVector2(const std::string& VariableName, float Vector2AsArray[2]) const;
    void SetVector2(const std::string& VariableName, const glm::vec2& Vec) const;
    void SetMat4(const std::string& VariableName, const glm::mat4& Matrix)const;
    void SetMat3(const std::string& VariableName, const glm::mat3& Matrix) const;

    void SetVector3(const std::string& VariableName,  const glm::vec3& Vector) const;



private:
    bool LoadShaderFromFile(const char* ShaderFilePath, std::string& OutShaderCode);
    unsigned int CompileShaderCode(const std::string& VertexShaderCode, const std::string& FragmentShaderCode);

    void ReportInvalidUniformName(const char* UniformName, const char* FunctionName, int LINE) const;

private:
    unsigned int ProgramID;
};

