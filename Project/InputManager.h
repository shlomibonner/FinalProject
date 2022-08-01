#pragma once   //maybe should be static class
#include "igl/opengl/glfw/Display.h"
#include "igl/opengl/glfw/renderer.h"
#include "Project.h"
#include "imgui/imgui.h"


bool leftMousePressed; 
double xBeginPos;
double yBeginPos;

float Normalize(float num, int magnitude)
{
	return (((num*2) / magnitude) - 1);
}

Eigen::Vector3f FindObjectPickedCenter(Project* scn)
{
	float centerZoom = 4;
	int shapesCount = scn->pShapes.size();

	if (shapesCount > 0)
	{

		Eigen::Vector3f avgPos = Eigen::Vector3f(0, 0, 0);

		for (int shapeIndex : scn->pShapes)
		{ // chaneg to picked shapes
			avgPos += scn->globalShapes[shapeIndex].currPos();
		}
		avgPos /= shapesCount;
		avgPos -= Eigen::Vector3f(0, 0, centerZoom);

		return avgPos;
	}
	return Eigen::Vector3f(0, 0, 0);
}

void scalePickedObject(double shift, directions d, Project* scn)
{
	for (int i : scn->pShapes)
	{
		scn->globalShapes[i].Scale(shift, d);
	}
}

bool inside(float xBeginPos, float yBeginPos, float xEndPos, float yEndPos, float screenX, float screenY)
{
	if (xBeginPos > xEndPos)
	{
		float tmp = xBeginPos;
		xBeginPos = xEndPos;
		xEndPos = tmp;
	}
	if (yBeginPos > yEndPos)
	{
		float tmp = yBeginPos;
		yBeginPos = yEndPos;
		yEndPos = tmp;
	}
	return (xBeginPos <= screenX && screenX <= xEndPos) && (yBeginPos <= screenY && screenY <= yEndPos);
	
}

void movePickedObjects(double shift, directions d, Project* scn)
{
	for (int i : scn->pShapes)
	{
		scn->globalShapes[i].move(shift, d);
	}
}


void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {

        Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
        Project* scn = (Project*)rndr->GetScene();
        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            rndr->Pressed();
            glfwGetCursorPos(window, &xBeginPos, &yBeginPos);
        }
        rndr->UpdatePress(xBeginPos, yBeginPos);
    }
    else
    {
        Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
        Project* scn = (Project*)rndr->GetScene();

        if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            rndr->UnPick(2);
            double xEnd, yEnd;
            glfwGetCursorPos(window, &xEnd, &yEnd);
            rndr->PickMany(2);
            rndr->Pressed();
        }
    }
}

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
    Project* scn = (Project*)rndr->GetScene();

    if (rndr->IsPicked())
    {
        rndr->UpdateZpos((int)yoffset);
        rndr->MouseProccessing(GLFW_MOUSE_BUTTON_MIDDLE);
    }
    else
    {
        rndr->MoveCamera(0, rndr->zTranslate, (float)yoffset);
    }
}

void glfw_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
    Project* scn = (Project*)rndr->GetScene();
    double xStart, yStart;

    rndr->UpdatePosition((float)xpos, (float)ypos);

    if (rndr->CheckViewport(xpos, ypos, 0))
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
        {
            rndr->UnPick(2);
            glfwGetCursorPos(window, &xStart, &yStart);
            if (!rndr->IsPressed())
            {
                rndr->Pressed();
            }
        }
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {

            rndr->MouseProccessing(GLFW_MOUSE_BUTTON_LEFT);
        }
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && rndr->IsPressed())
        {
            double xEnd, yEnd;
            glfwGetCursorPos(window, &xEnd, &yEnd);
            rndr->PickMany(2);
            rndr->Pressed();
        }
    }
}

void glfw_window_size_callback(GLFWwindow* window, int width, int height)
{
    Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);

    rndr->resize(window, width, height);
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Renderer* rndr = (Renderer*)glfwGetWindowUserPointer(window);
    Project* scn = (Project*)rndr->GetScene();
    // rndr->FreeShapes(2);
    if (action == GLFW_PRESS || action == GLFW_REPEAT)
    {
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;

        case GLFW_KEY_SPACE:
            if (scn->IsActive())
                scn->Deactivate();
            else
                scn->Activate();
            break;

        case GLFW_KEY_UP:
            rndr->MoveCamera(0, scn->xRotate, 0.05f);

            break;
        case GLFW_KEY_DOWN:
            
            rndr->MoveCamera(0, scn->xRotate, -0.05f);
            break;
        case GLFW_KEY_LEFT:
            rndr->MoveCamera(0, scn->yRotate, 0.05f);
            break;
        case GLFW_KEY_RIGHT:
            
            rndr->MoveCamera(0, scn->yRotate, -0.5f);
            break;
        case GLFW_KEY_U:
            rndr->MoveCamera(0, scn->yTranslate, 0.25f);
            break;
        case GLFW_KEY_D:
            movePickedObjects(0.02, x, scn);
            break;
        case GLFW_KEY_A:
            movePickedObjects(-0.02, x, scn);
            break;
        case GLFW_KEY_W:
            movePickedObjects(0.02, y, scn);
            break;
        case GLFW_KEY_S:
            movePickedObjects(-0.02, y, scn);
            break;
        case GLFW_KEY_L:
            rndr->MoveCamera(0, scn->xTranslate, -0.25f);
            break;
        case GLFW_KEY_K:
            scn->NextCubeMap();
            break;
        case GLFW_KEY_R:
            rndr->MoveCamera(0, scn->xTranslate, 0.25f);
            break;

        case GLFW_KEY_B:
            rndr->MoveCamera(0, scn->zTranslate, 0.5f);
            break;
        case GLFW_KEY_F:
            rndr->MoveCamera(0, scn->zTranslate, -0.5f);
            break;
        case GLFW_KEY_T:
        {
            rndr->MoveCamera(0, 100, 0);
            break;
        }
        case GLFW_KEY_O:
        {
            Eigen::Vector3f center = FindObjectPickedCenter(scn);
            rndr->MoveCamera(0, 100, 0);
            rndr->MoveCamera(0, scn->xTranslate, center.x());
            rndr->MoveCamera(0, scn->yTranslate, center.y());
            rndr->MoveCamera(0, scn->zTranslate, center.z());
           
            break;
        }
        case GLFW_KEY_X:
            movePickedObjects(+0.02, z, scn);
            break;
        case GLFW_KEY_Z:
            movePickedObjects(-0.02, z, scn);
            break;
        case GLFW_KEY_1:
            scalePickedObject(1.2, x, scn);
            break;
        case GLFW_KEY_2:
            scalePickedObject(0.8, x, scn);
            break;
        case GLFW_KEY_3:
            scalePickedObject(1.2, y, scn);
            break;
        case GLFW_KEY_4:
            scalePickedObject(0.8, y, scn);
            break;
        case GLFW_KEY_5:
            scalePickedObject(1.2, z, scn);
            break;
        case GLFW_KEY_6:
            scalePickedObject(0.8, z, scn);
            break;
        case GLFW_KEY_EQUAL:
            scalePickedObject(1.2, w, scn);
            break;
        case GLFW_KEY_MINUS:
            scalePickedObject(0.8, w, scn);
            break;
        default:
            break;
        }
    }
}

void Init(Display& display, igl::opengl::glfw::imgui::ImGuiMenu* menu)
{
    display.AddKeyCallBack(glfw_key_callback);
    display.AddMouseCallBacks(glfw_mouse_callback, glfw_scroll_callback, glfw_cursor_position_callback);
    display.AddResizeCallBack(glfw_window_size_callback);
    menu->init(&display);
}