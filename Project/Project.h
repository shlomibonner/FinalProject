#pragma once
#include "igl/opengl/glfw/Viewer.h"
#include "MovableShape.h"
#include "igl/opengl/glfw/renderer.h"

class Project : public igl::opengl::glfw::Viewer
{
	
public:
	
    float time = 0;
    std::vector<MovableShape> globalShapes;
    int cubeMapIndx = -1;
    int pickingPlaneIndx = -1;
    int transShaderIndx = -1;
    int basicShaderIndx = -1;
    float maxTime = 1;
    long globalTime;
    std::vector<Layer*> layers;



	Project();
//	Project(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int  shaderIndx, unsigned int shapeIndx);
	void WhenRotate();
	void WhenTranslate();
	void Animate() override;
	void ScaleAllShapes(float amt, int viewportIndx);
    void SetRenderer(Renderer* renderer);
    void SetParent(int shape, int newParent);
    int GetParent(int shape);
    void SetMenu(igl::opengl::glfw::imgui::ImGuiMenu* menu);
    igl::opengl::glfw::imgui::ImGuiMenu* menu;
    std::vector<int> GetChildren(int shape);
    bool Load_Shape_From_File(const std::string& mesh_file_name_string);
    IGL_INLINE void my_open_dialog_load_mesh();
    void Play();
    
    
    ~Project(void);
    void ChangeCubeMap(std::string file_name);
    void NextCubeMap();
    
private:
    Renderer* renderer = nullptr;
    int AddGlobalShapeFromFile(std::string name, std::string file_name, int parent, Viewer* viewer);
    int AddGlobalShape(std::string name, igl::opengl::glfw::Viewer::shapes shapeType, Viewer* viewer, int parent, int viewPort);

    bool animating;
	
};


