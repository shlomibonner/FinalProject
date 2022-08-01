#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "InputManager.h"
#include "MenuGUI.h"

int main(int argc, char* argv[])
{
    const int DISPLAY_WIDTH = 1200;
    const int DISPLAY_HEIGHT = 800;
    const float CAMERA_ANGLE = 45.0f;
    const float NEAR = 1.0f;
    const float FAR = 120.0f;
    const int infoIndx = 2;
    std::list<int> x, y;
    x.push_back(DISPLAY_WIDTH);
    y.push_back(DISPLAY_HEIGHT);
    Display disp = Display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "FINAL_PROJECT");
    igl::opengl::glfw::imgui::ImGuiMenu* menu = new igl::opengl::glfw::imgui::ImGuiMenu();
    Renderer* rndr = new Renderer(CAMERA_ANGLE, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, NEAR, FAR);
    Project* scn = new Project(); // initializing scene

    scn->SetMenu(menu);
    Init(disp, menu);               // adding callback functions
    scn->Init();                    // adding shaders, textures, shapes to scene
    rndr->Init(scn, x, y, 1, menu); // adding scene and viewports to the renderer
    scn->SetRenderer(rndr);
    disp.SetRenderer(rndr);

    rndr->AddViewport(-1, 1, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    rndr->AddViewport(-1, 1, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    rndr->CopyDraw(1, rndr->viewport, 1);
    rndr->ClearDrawFlag(2, rndr->toClear | rndr->stencilTest);
    rndr->SetDrawFlag(2, rndr->blend | rndr->inAction2 | rndr->scissorTest);

    rndr->AddDraw(2, 0, 3, 0, rndr->scaleAbit | rndr->depthTest | rndr->onPicking);

    
    disp.launch_rendering(rndr);

    delete scn;
    delete rndr;
    delete menu;

    return 0;
}