#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

#include <Shader.h>
#include <Texture.h>

#include <CMakeProjectMacros.h>

#include <vector>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

#pragma warning(disable : 4996) //suppress 'sprintf' warning or use 'sprintf_s'

// settings
unsigned int SCR_WIDTH  = 800;
unsigned int SCR_HEIGHT = 600;

std::ostream& operator<<(std::ostream& cout, const glm::vec2& vector);
std::ostream& operator<<(std::ostream& cout, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& cout, const glm::vec4& vector);
std::ostream& operator<<(std::ostream& cout, const glm::mat3& Matrix);
std::ostream& operator<<(std::ostream& cout, const glm::mat4& Matrix);

glm::vec2 PlaneVertexData[]
{
    //Positions             //Texture co-ordinates
    glm::vec2(-1.f, +1.f), glm::vec2(0.f, 1.f),
    glm::vec2(-1.f, -1.f), glm::vec2(0.f, 0.f),
    glm::vec2(+1.f, -1.f), glm::vec2(1.f, 0.f),
    glm::vec2(+1.f, +1.f), glm::vec2(1.f, 1.f)
};

unsigned int PlaneDrawIndices[]
{
    0, 1, 2,
    2, 3, 0
};

glm::vec2 MousePosition  = glm::vec2(0);

struct FBone
{
	glm::vec2  Position = glm::vec2(0);
	float Length     = 100;
	float Rotation   = 90;
	glm::vec2  End   = glm::vec2(0);
	glm::vec3  Color = glm::vec3(0.1f);

    static float BoneBreadth;
};
float FBone::BoneBreadth = 10.f;

glm::vec2 RemapMouseToGraphDimension( glm::vec2 MousePosition, float GraphSize );
void UpdateBonesToTarget(std::vector<FBone>& Bones, glm::vec2 TargetPosition,  bool FixedBase = true);
void SendBonesToShader(std::vector<FBone>& Bones, const Shader& Program);

void AddBone( std::vector<FBone>& BoneChain, const FBone& NewBone);



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Inverse Kinematics", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   


    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertexData), PlaneVertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlaneDrawIndices), PlaneDrawIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2  * sizeof(PlaneVertexData[0]), (void*)0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(PlaneVertexData[0]), (void*)(sizeof(PlaneVertexData[0])));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    float DeltaTime = 0;
    float LastTime  = 0;

    Shader Program_Quad1(
        PROJECT_DIR"/ShaderPrograms/IK.vert", 
        PROJECT_DIR"/ShaderPrograms/IK.frag"
    );

    

    std::cout << std::boolalpha;
    glfwSetKeyCallback(window, [](GLFWwindow* WINDOW, int Key, int scancode, int keystate, int mods)
    {
        
    });

    glfwSetScrollCallback(window, [](GLFWwindow* Window, double XOffset, double YOffset)
    {
       
    });

    

    glfwSetCursorPosCallback(window, [](GLFWwindow* Window, double MouseX, double MouseY)
    {
        MousePosition = glm::vec2(MouseX, MouseY);
    });



    //First time case
    glm::vec2 MouseCurrent, MousePrevious = glm::vec2(0), MouseDelta = glm::vec2(0);

    //Bones
    std::vector<FBone> Bones; Bones.reserve(5); FBone BoneTmp;

    //Add bones
    BoneTmp.Color = glm::vec3(1.0, 0.0, 0.0);
    AddBone(Bones, BoneTmp);

    BoneTmp.Color = glm::vec3(0.0, 1.0, 0.0);
    AddBone(Bones, BoneTmp);

    BoneTmp.Color = glm::vec3(0.0, 0.0, 1.0);
    AddBone(Bones, BoneTmp);

    BoneTmp.Color = glm::vec3(0.0, 1.0, 1.0);
    AddBone(Bones, BoneTmp);

    Program_Quad1.Use();
    SendBonesToShader(Bones, Program_Quad1);

    //
    //Draw Graph Size
    float GraphSize = 1000;
    FBone::BoneBreadth = 6;
    Program_Quad1.SetFloat("UBoneBreadth", FBone::BoneBreadth );

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0, 0.5f, 0.5f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        Program_Quad1.Use();
        Program_Quad1.SetVector2("UWindowDimension", glm::vec2(SCR_WIDTH * 1.f, SCR_HEIGHT * 1.f) );
        Program_Quad1.SetFloat("UGraphSize", GraphSize );
        Program_Quad1.SetVector2("UMousePosition", MousePosition);

        // //Delay
        MouseDelta.x     = MousePosition.x - MousePrevious.x;
        MouseDelta.y     = MousePosition.y - MousePrevious.y;

        MouseCurrent.x   = MousePrevious.x + MouseDelta.x * 0.1f;
        MouseCurrent.y   = MousePrevious.y + MouseDelta.y * 0.1f;

        MousePrevious.x  = MouseCurrent.x;
        MousePrevious.y  = MouseCurrent.y;
        
        if( glm::length2(MouseDelta) > .5f ) //if mouse delta is less than this. No  need to update. 
        {
            //std::cout << "Delta is greater than threshold \t Time = " << glfwGetTime() << "s\n";

            UpdateBonesToTarget(
                Bones, 
                RemapMouseToGraphDimension(MouseCurrent, GraphSize),
                true
             );

            SendBonesToShader(Bones, Program_Quad1);
        }
        
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        float CurrentTime = (float)glfwGetTime();
        DeltaTime = CurrentTime - LastTime;
        LastTime  = CurrentTime;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void AddBone( std::vector<FBone>& Bones, const FBone& NewBone)
{
    if( Bones.size() == 0)
    {
        Bones.emplace_back(NewBone);
        Bones[0].End      = NewBone.Position 
                          + glm::vec2( 
                              cos( glm::radians(NewBone.Rotation) ) *  NewBone.Length, 
                              sin( glm::radians(NewBone.Rotation) ) *  NewBone.Length 
                            );

        std::cout << "i = 0\n";
        return;
    }

    size_t i = Bones.size(); 
	Bones.emplace_back(NewBone);

    Bones[i].Position = Bones[i - 1].End;
    Bones[i].End      = Bones[i].Position 
                      + glm::vec2( 
                          cos( glm::radians(NewBone.Rotation) ) *  NewBone.Length, 
                          sin( glm::radians(NewBone.Rotation) ) *  NewBone.Length 
                        );

    std::cout << "i = " << i << "\n";
}

glm::vec2 RemapMouseToGraphDimension( glm::vec2 MousePosition, float GraphSize )
{
    glm::vec2   WindowDimension( SCR_WIDTH, SCR_HEIGHT);
    const float AspectRatio = WindowDimension.x / WindowDimension.y;

    //Remapping mouse position to graph dimension
    MousePosition.x    = ((MousePosition.x / WindowDimension.x) - 0.5f) / 0.5f;
    MousePosition.y    = ((MousePosition.y / WindowDimension.y) - 0.5f) / 0.5f;

	MousePosition.x   *= GraphSize * 0.5f;
    MousePosition.y   *= GraphSize * 0.5f;

	MousePosition.x *= AspectRatio;
	MousePosition.y *= -1.f;

    return MousePosition;
}

void UpdateBonesToTarget(std::vector<FBone>& Bones, glm::vec2 TargetPosition, bool FixedBase)
{
    using namespace glm;

    if( Bones.size() <= 0 ) return;

    const int TotalBones     = Bones.size();

	glm::vec2  ToTarget     = TargetPosition - Bones[TotalBones - 1].Position;
	float ToTarget_Rotation = glm::atan( ToTarget.y, ToTarget.x );

	Bones[TotalBones - 1].Position = TargetPosition + ( glm::normalize(ToTarget) * -Bones[TotalBones - 1].Length);
	Bones[TotalBones - 1].Rotation = glm::degrees( ToTarget_Rotation );

	for(int i = TotalBones - 2; (FixedBase ? i > 0 : i >= 0) ; i--)  //for a fixed base change " i >= 0 " to " i > 0 "
	{
		TargetPosition = Bones[i + 1].Position;

		ToTarget   = TargetPosition - Bones[i].Position;
		ToTarget_Rotation = glm::atan( ToTarget.y, ToTarget.x );

		Bones[i].Position = TargetPosition + ( normalize(ToTarget) * -Bones[i].Length);
		Bones[i].Rotation = glm::degrees( ToTarget_Rotation );
		Bones[i].End = Bones[i].Position + normalize(ToTarget) * Bones[i].Length;

	}

//Fixed base starts here
    if( FixedBase )
    {

        ToTarget   = Bones[1].Position - Bones[0].Position;
        ToTarget_Rotation = atan( ToTarget.y, ToTarget.x );

        Bones[0].Rotation = degrees( ToTarget_Rotation );
        Bones[0].End 	  = Bones[0].Position + normalize(ToTarget) * Bones[0].Length;


        for(int i = 1; i < TotalBones; i++)
        {
        
            Bones[i].Position.x = Bones[i - 1].End.x;
            Bones[i].Position.y = Bones[i - 1].End.y;

            Bones[i].End.x = Bones[i].Position.x + cos( radians(Bones[i].Rotation) ) * Bones[i].Length;
            Bones[i].End.y = Bones[i].Position.y + sin( radians(Bones[i].Rotation) ) * Bones[i].Length;
        }

    }
}

void SendBonesToShader(std::vector<FBone>& Bones, const Shader& Program)
{
    int i = 0;
    for (const auto Bone : Bones)
    {
        char buffer[64]{};

        sprintf(buffer, "UBones[%i].Position", i);
        Program.SetVector2(buffer, Bone.Position );

        sprintf(buffer, "UBones[%i].Length", i);
        Program.SetFloat(buffer, Bone.Length );

        sprintf(buffer, "UBones[%i].Rotation", i);
        Program.SetFloat(buffer, Bone.Rotation );

        sprintf(buffer, "UBones[%i].End", i);
        Program.SetVector2(buffer, Bone.End );

        sprintf(buffer, "UBones[%i].Color", i);
        Program.SetVector3(buffer, Bone.Color );
        
        i++;
    }

    Program.SetInt("UTotalBones",    Bones.size() );
    Program.SetFloat("UBoneBreadth", FBone::BoneBreadth );
}



void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH  = width;
    SCR_HEIGHT = height;
}

std::ostream& operator<<(std::ostream& cout, const glm::vec2& vector)
{
    printf("< %3.2f, %3.2f >", vector.x, vector.y);
    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::vec3& vector)
{
    printf("< %3.2f, %3.2f, %3.2f >", vector.x, vector.y, vector.z);
    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::vec4& vector)
{
    printf("< %3.2f, %3.2f, %3.2f, %3.2f >", vector.x, vector.y, vector.z, vector.w);

    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::mat3& Matrix)
{
    std::cout << "========================================MATRIX==========================================\n";
    printf("   X|   Y|    Z|  \n");
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][0], Matrix[1][0], Matrix[2][0] );
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][1], Matrix[1][1], Matrix[2][1] );
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][2], Matrix[1][2], Matrix[2][2] );
    cout << "-------------------------------\n";
    cout << "X = " << Matrix[0] << "\n";
    cout << "Y = " << Matrix[1] << "\n";
    cout << "Z = " << Matrix[2] << "\n";
    std::cout << "=======================================================================================\n";

    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::mat4& Matrix)
{
    std::cout << "========================================MATRIX==========================================\n";
    printf("   X|   Y|    Z|     W|  \n");
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][0], Matrix[1][0], Matrix[2][0], Matrix[3][0] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][1], Matrix[1][1], Matrix[2][1], Matrix[3][1] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][2], Matrix[1][2], Matrix[2][2], Matrix[3][2] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][3], Matrix[1][3], Matrix[2][3], Matrix[3][3] );
    cout << "-------------------------------\n";
    cout << "X = " << Matrix[0] << "\n";
    cout << "Y = " << Matrix[1] << "\n";
    cout << "Z = " << Matrix[2] << "\n";
    cout << "W = " << Matrix[3] << "\n";
    std::cout << "=======================================================================================\n";


    return cout;
}