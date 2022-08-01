#ifndef PROJECT_MOVABLESHAPE_H
#define PROJECT_MOVABLESHAPE_H

#include <Eigen/Core>
#include "igl/opengl/glfw/Viewer.h"

enum directions {
    x, y, z, w
};

class Layer {
public:
    Layer(bool isVisible, std::string layerName, std::vector<int> layerShapes);
    std::string getName();
    void setName(std::string name);
    bool getVisible();
    void setVisible(bool vis);
    void addShape(int shapeIndex);
    void deleteShape(int shapeIndex);
    std::vector<int> getLayerShapes();
private:
    bool visible;
    std::string layerName;
    std::vector<int> layerShapes;
};

class BezierMovement {
public:
    BezierMovement(std::vector<Eigen::Vector3f> movement, float startTime, float endTime) :
        points(movement), startTime(startTime), endTime(endTime) {};
    std::vector<Eigen::Vector3f> points;
    float startTime;
    float endTime;
    Eigen::Vector3f getPos(float time) {
        size_t n = points.size();
        if (n == 1)
            return points[0];
        float normalizedTime = (time - startTime) / (endTime - startTime);
        Eigen::Vector3f pos = Eigen::Vector3f(0, 0, 0);
        for (std::size_t i = 0; i < n; i++) {
            pos += BinomialCoeff(n, i) * pow((1 - normalizedTime), n - i) * pow(normalizedTime, i) * points[i];
        }
        return pos;
    };

private:
    int BinomialCoeff(const int n, const int k) {
        if (k == 0)
            return 1;
        std::vector<int> SOL(k);
        SOL[0] = n - k + 1;

        for (int i = 1; i < k; ++i) {
            SOL[i] = SOL[i - 1] * (n - k + 1 + i) / (i + 1);
        }

        return SOL[k - 1];
    }
};

class MovableShape {
public:

    bool transparent = false;
    std::vector<MovableShape*> getChildren();

    MovableShape(std::string shapeName, igl::opengl::glfw::Viewer::shapes shapeType,
        Layer* layer, int index, igl::opengl::glfw::Viewer* viewer);

    Layer* getLayer();
    void changeLayer(Layer* layer);
    int getIndex();
    void bezierAdd(BezierMovement bezi, float* max_time);
    Eigen::Vector3f getPos(float time);
    Eigen::Vector3f getlastDrawnPosition();
    void setlastDrawnPosition(Eigen::Vector3f pos);
    std::string getName() { return name; };
    MovableShape* getParent();
    void removeParent();
    void setParent(MovableShape* newParent);
    void addChild(MovableShape* child);
    void removeChild(MovableShape* child);
    
    Eigen::Vector3f currPosAt(float time);
    Eigen::Vector3f currPos();
    
    void MovableShape::Scale(double shiftSize, directions d);

    //edit mode movment:
    void move(double shiftSize, directions d);

private:
    std::string name;
    igl::opengl::glfw::Viewer::shapes type;
    std::vector<BezierMovement> movement;
    Eigen::Vector3f currentPosition;
    igl::opengl::glfw::Viewer* viewer;
    Layer* layer;
    int index;
    Eigen::Vector3f lastDrawnPosition;
    MovableShape* parent;
    std::vector<MovableShape*> children;


};
#endif