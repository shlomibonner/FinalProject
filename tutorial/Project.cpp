#include "Project.h"
#include <iostream>
#include <chrono>
#include "../../igl/file_dialog_open.h"
#include <igl/opengl/glfw/renderer.h>

Layer* defaultLayer = new Layer(true, "default", std::vector<int>());

static void printMat(const Eigen::Matrix4d& mat)
{
    std::cout << " matrix:" << std::endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            std::cout << mat(j, i) << " ";
        std::cout << std::endl;
    }
}

IGL_INLINE void Project::my_open_dialog_load_mesh()
{
    std::string fname = igl::file_dialog_open();

    if (fname.length() == 0)
        return;

    Load_Shape_From_File(fname.c_str());
}

int fileIndex = 0;

bool Project::Load_Shape_From_File(
    const std::string& mesh_file_name_string)
{
    std::vector<Eigen::Vector3f> points = { Eigen::Vector3f(7, 7, 0),
                                           Eigen::Vector3f(0, 20, 0),
                                           Eigen::Vector3f(-5, -5, -75),
                                           Eigen::Vector3f(7, 7, 0) };

    int index = AddGlobalShapeFromFile("file: " + fileIndex++, mesh_file_name_string, -1, this);
    SetShapeShader(index, 2);
    SetShapeMaterial(index, 2);
    std::cout << "Loading mesh from a file: " << mesh_file_name_string << std::endl;
    return true;
}

long getCurrentUnixTime()
{
    const auto p1 = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(p1.time_since_epoch()).count();
}

Project::Project() {}

void Project::SetMenu(igl::opengl::glfw::imgui::ImGuiMenu* menu)
{
    this->menu = menu;
}

int Project::AddGlobalShapeFromFile(std::string name, std::string file_name, int parent, Viewer* viewer)
{
    int index = AddShapeFromFile(file_name, parent, TRIANGLES);
    MovableShape scnShape(name, MeshCopy, defaultLayer, index, viewer);
    scnShape.setlastDrawnPosition(Eigen::Vector3f(0, 0, 0));
    globalShapes.push_back(scnShape);
    return index;
}

int Project::AddGlobalShape(std::string name, igl::opengl::glfw::Viewer::shapes shapeType,
    Viewer* viewer, int parent, int viewPort = 0)
{
    int index = AddShape(shapeType, parent, TRIANGLES, viewPort);
    MovableShape scnShape(name, shapeType, defaultLayer, index, viewer);
    scnShape.setlastDrawnPosition(Eigen::Vector3f(0, 0, 0));
    globalShapes.push_back(scnShape);
    return index;
}

void Project::Init()
{
    layers.push_back(defaultLayer);
    globalTime = 0;
    unsigned int texIDs[4] = { 0, 1, 2, 3 };
    unsigned int slots[4] = { 0, 1, 2, 3 };
    int pickingShaderIndx = AddShader("../../shaders/pickingShader");
    int cubemapShaderIndx = AddShader("../../shaders/cubemapShader");
    basicShaderIndx = AddShader("../../shaders/basicShader");
    transShaderIndx = AddShader("../../shaders/transShader");

    unsigned int snakeTexIndx = AddTexture("../../textures/snake.jpg", 2);
    unsigned int dayLightBoxTexIndx = AddTexture("../../textures/cubemaps/Daylight Box_", 3);
    unsigned int boxTexIndx = AddTexture("../../textures/box0.bmp", 2);
    unsigned int planeTexIndx = AddTexture("../../textures/plane.png", 2);
    

    int snake2DMatIndx = AddMaterial(&snakeTexIndx, slots + 3, 1);
    int dayLight3DMatIndx = AddMaterial(&dayLightBoxTexIndx, slots + 1, 1);

    int box2DMatIndx = AddMaterial(&boxTexIndx, slots + 2, 1);
    int plane2DMatIndx = AddMaterial(&planeTexIndx, slots + 3, 1);
    

    std::vector<Eigen::Vector3f> points = { Eigen::Vector3f(0, 2, 0),
                                           Eigen::Vector3f(0, 5, 0),
                                           Eigen::Vector3f(-7.5, -7.5, -160),
                                           Eigen::Vector3f(0, -5, 0) };

    std::vector<Eigen::Vector3f> pointsRev = { Eigen::Vector3f(0, -5, 0),
                                              Eigen::Vector3f(-7.5, -7.5, -160),
                                              Eigen::Vector3f(0, 5, 0),
                                              Eigen::Vector3f(0, 2, 0) };
    std::vector<Eigen::Vector3f> point = { Eigen::Vector3f(0, 3, 0) };


    cubeMapIndx = AddGlobalShape("cubeMap", Cube, this, -2);
    SetShapeShader(cubeMapIndx, cubemapShaderIndx);
    SetShapeMaterial(cubeMapIndx, dayLight3DMatIndx);

    ShapeTransformation(scaleAll, 150, 0);
    SetShapeStatic(cubeMapIndx);


    
    // Picking plane


    pickingPlaneIndx = AddGlobalShape("Picking plane", Plane, this, -2, 1);
    SetShapeShader(pickingPlaneIndx, pickingShaderIndx);
    SetShapeMaterial(pickingPlaneIndx, plane2DMatIndx);

    
    ShapeTransformation(zTranslate, -1.3, 1);

    SetShapeStatic(pickingPlaneIndx);

    // Picking plane End
    int index;
    index = AddGlobalShape("testCyl", xCylinder, this, -1);
    SetShapeShader(index, basicShaderIndx);
    SetShapeMaterial(index, box2DMatIndx);
    globalShapes[index].bezierAdd(BezierMovement(points, 0, 700), &maxTime);
    globalShapes[index].bezierAdd(BezierMovement(point, 700, 1700), &maxTime);
    globalShapes[index].bezierAdd(BezierMovement(pointsRev, 1700, 2500), &maxTime);
    globalShapes[index].move(2, y);


    index = AddGlobalShape("testCube", Cube, this, -1);
    SetShapeShader(index, basicShaderIndx);
    SetShapeMaterial(index, box2DMatIndx);
    globalShapes[index].move(2, x);


    index = AddGlobalShape("testSphere", Sphere, this, -1);
    SetShapeShader(index, basicShaderIndx);
    SetShapeMaterial(index, box2DMatIndx);

    index = AddGlobalShape("testCyl", xCylinder, this, -1);
    SetShapeShader(index, basicShaderIndx);
    SetShapeMaterial(index, snake2DMatIndx);
    globalShapes[index].move(2, x);


    globalShapes[selected_data_index].move(-1, y);

}

void Project::Update(const Eigen::Matrix4f& Proj, const Eigen::Matrix4f& View, const Eigen::Matrix4f& Model, unsigned int shaderIndx, unsigned int shapeIndx)
{
    Shader* s = shaders[shaderIndx];
    long ctime;
    if (isActive) {
        ++globalTime;
        ctime = globalTime;
    }
    else
        ctime = time;

    if (shapeIndx != cubeMapIndx && shapeIndx != pickingPlaneIndx) {
        MovableShape scnShape = globalShapes[shapeIndx];
        Eigen::Vector3f pos = scnShape.getlastDrawnPosition();
        Eigen::Vector3f newPos = scnShape.getPos((float)ctime);
        if (newPos != pos) {

            Eigen::Vector3f d = newPos - pos;
            selected_data_index = shapeIndx;
            ShapeTransformation(xTranslate, d(x), 0);
            ShapeTransformation(yTranslate, d(y), 0);
            ShapeTransformation(zTranslate, d(z), 0);
            globalShapes[shapeIndx].setlastDrawnPosition(newPos);
        }
    }


    s->Bind();
    s->SetUniformMat4f("Proj", Proj);
    s->SetUniformMat4f("View", View);
    s->SetUniformMat4f("Model", Model);
    if (!globalShapes[shapeIndx].getLayer()->getVisible()) {
        s->SetUniform2f("trans", 0, 0);
    }
    else 
        globalShapes[shapeIndx].transparent ? s->SetUniform2f("trans", 0.6, 0) : s->SetUniform2f("trans", 1, 0);
       

    if (data_list[shapeIndx]->GetMaterial() >= 0 && !materials.empty())
    {
        BindMaterial(s, data_list[shapeIndx]->GetMaterial());
    }
    s->Unbind();
}

void Project::WhenRotate()
{
}

void Project::WhenTranslate()
{
}

const int numOfCubeMaps = 3;

std::string cubeMaps[numOfCubeMaps] = { "Daylight Box_", "grass_cubemap_",
                                       "desert_cubemap_" };

int cubeMapCurrTextIndx = 0;

void Project::NextCubeMap()
{
    cubeMapCurrTextIndx = (cubeMapCurrTextIndx + 1) % numOfCubeMaps;
    ChangeCubeMap(cubeMaps[cubeMapCurrTextIndx]);
}

void Project::ChangeCubeMap(std::string file_name)
{
    unsigned int tex = AddTexture("../../textures/cubemaps/" + file_name, 3);
    unsigned int numOfMats = materials.size();
    int cubeMat = AddMaterial(&tex, &numOfMats, 1);
    SetShapeMaterial(cubeMapIndx, cubeMat);
}

void Project::Animate()
{
    animating = !animating;
}

void Project::ScaleAllShapes(float amt, int viewportIndx)
{
    for (int i = 1; i < data_list.size(); i++)
    {
        if (data_list[i]->Is2Render(viewportIndx))
        {
            data_list[i]->MyScale(Eigen::Vector3d(amt, amt, amt));
        }
    }
}

Project::~Project(void)
{
}

void Project::SetRenderer(Renderer* renderer)
{
    if (this->renderer != nullptr)
        throw std::invalid_argument("Cannot set renderer twice.");
    this->renderer = renderer;
}

void Project::Play()
{
    animating = !animating;
    globalTime = -1;
}