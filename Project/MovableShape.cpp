//
// Created by ditto on 28/06/2022.
//

#include "MovableShape.h"

// Layer ------>
Layer::Layer(bool visible, std::string layerName, std::vector<int> layerShapes) : visible(visible), layerName(layerName), layerShapes(layerShapes) {}

std::string Layer::getName()
{
    return this->layerName;
}

void Layer::setName(std::string name)
{
    this->layerName = name;
}

bool Layer::getVisible()
{
    return this->visible;
}

void Layer::setVisible(bool toSet)
{
    this->visible = toSet;
}
std::vector<int> Layer::getLayerShapes()
{
    return this->layerShapes;
}

void Layer::addShape(int shapeIndex) {
    this->layerShapes.push_back(shapeIndex);
}

void Layer::deleteShape(int shapeIndex) {
    std::vector<int> layerShapes;
    for (int num : this->layerShapes) {
        if (num != shapeIndex)
            layerShapes.push_back(num);
    }
    this->layerShapes = layerShapes;

}
//END Layer ------->

MovableShape::MovableShape(std::string shapeName, igl::opengl::glfw::Viewer::shapes shapeType,
    Layer* layer, int index, igl::opengl::glfw::Viewer* viewer) : name(shapeName), type(shapeType),
    layer(layer), index(index), currentPosition(Eigen::Vector3f(0, 0, 0)), viewer(viewer)
{
    movement = std::vector<BezierMovement>();
}

Layer* MovableShape::getLayer()
{
    return layer;
}
void MovableShape::changeLayer(Layer* layer)
{
    this->layer = layer;
}
int MovableShape::getIndex() {
    return index;
}


void MovableShape::bezierAdd(BezierMovement bezi, float* max_time) {
    this->movement.push_back(bezi);
    if (bezi.endTime > *max_time)
        *max_time = bezi.endTime;
}


Eigen::Vector3f MovableShape::getlastDrawnPosition() {
    return lastDrawnPosition;
}

void MovableShape::setlastDrawnPosition(Eigen::Vector3f pos) {
    lastDrawnPosition = pos;
}


MovableShape* MovableShape::getParent() {
    return parent;
}

void MovableShape::removeParent()
{
    currentPosition = currPosAt(0);
    parent = nullptr;

}


void MovableShape::setParent(MovableShape* newParent) {
    parent = newParent;
    currentPosition = currentPosition - newParent->currPos();
    parent->addChild(this);
}

std::vector<MovableShape*> MovableShape::getChildren() {
    return children;
}

Eigen::Vector3f MovableShape::currPosAt(float time)
{
    if (parent == nullptr)
        return getPos(time);
    else
        return (getPos(time) + this->parent->currPosAt(time));
}

Eigen::Vector3f MovableShape::currPos() {
    return currentPosition;
}


void MovableShape::addChild(MovableShape* child) {
    children.push_back(child);
}

void MovableShape::removeChild(MovableShape* removedChild) {
    std::vector<MovableShape*> newChildren;
    for (MovableShape* child : children) {
        if (child != removedChild)
            newChildren.push_back(child);
    }
    children = newChildren;
}

///move object

Eigen::Vector3f MovableShape::getPos(float time) {
    for (std::size_t i = 0; i < this->movement.size(); i++) {
        if (movement[i].startTime <= time && movement[i].endTime >= time) {
            return movement[i].getPos(time);
        }
    }
    return lastDrawnPosition;
}
void MovableShape::Scale(double shift, directions dir) {
    switch (dir)
    {
    case x:
        viewer->data_list[getIndex()]->MyScale(Eigen::Vector3d(shift, 1, 1));
        break;
    case y:
        viewer->data_list[getIndex()]->MyScale(Eigen::Vector3d(1, shift, 1));
        break;

    case z:
        viewer->data_list[getIndex()]->MyScale(Eigen::Vector3d(1, 1, shift));
        break;

    case w:
        viewer->data_list[getIndex()]->MyScale(Eigen::Vector3d(shift, shift, shift));
        break;
    }
}

void MovableShape::move(double shift, directions dir) {
    switch (dir) {
    case x:
        viewer->data_list[getIndex()]->MyTranslate(Eigen::Vector3d(shift, 0, 0), false);
        currentPosition += Eigen::Vector3f(shift, 0, 0);
        break;

    case y:
        viewer->data_list[getIndex()]->MyTranslate(Eigen::Vector3d(0, shift, 0), false);
        currentPosition += Eigen::Vector3f(0, shift, 0);
        break;

    case z:
        viewer->data_list[getIndex()]->MyTranslate(Eigen::Vector3d(0, 0, shift), false);
        currentPosition += Eigen::Vector3f(0, 0, shift);
        break;
    }

}