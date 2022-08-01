#include "Project.h"

class MenuGUI
{
public:
    enum CameraSplitMode
    {
        no_split,
        split_x,
        split_y
    };

    static void OnCameraSplitChange(int splitMode, Project* scn)
    {
        switch (splitMode)
        {
        case CameraSplitMode::no_split:
            // couldn't implement
            break;
        case CameraSplitMode::split_x:
            // couldn't implement
            break;
        case CameraSplitMode::split_y:
            // couldn't implement
            break;
        default:
            // couldn't implement
            break;
        }
    }

    static void OnTransparentToggled(bool transparent, Project* scn)
    {
        for (size_t i = 0; i < scn->pShapes.size(); i++)
        {
            int shapeIndx = scn->pShapes[i];
            scn->globalShapes[shapeIndx].transparent = transparent;
        }
    }

    static void OnBlurToggled(bool isBlurred, Project* scn)
    {
        for (size_t i = 0; i < scn->pShapes.size(); i++)
        {
            int shapeIndx = scn->pShapes[i];
            if (isBlurred == true)
            {
                scn->SetShapeShader(shapeIndx, scn->transShaderIndx);
            }
            else
            {
                scn->SetShapeShader(shapeIndx, scn->basicShaderIndx);
            }
        }
    }

    static void OnAddCamera(std::string camera_name, Project* scn)
    {
        std::cout << "Couldn't implement correctly" << std::endl;
    }

    static void OnAddLayer(std::string layer_name, bool is_shown, Project* scn)
    {
        std::vector<int> layerShapes;
        Layer* layer = new Layer(is_shown, layer_name, layerShapes);
        scn->layers.push_back(layer);
    }

    static void OnLayerChange(std::string layer_name, bool is_visible, Project* scn)
    {
        for (Layer* layer : scn->layers)
        {
            if (layer->getName() == layer_name)
            {
                layer->setVisible(is_visible);
                break;
            }
        }
    }

    static void OnSetLayer(std::string layer_name, Project* scn)
    {
        std::cout << "layer: " << layer_name << ", picked: " << scn->pShapes.size() << std::endl;
        Layer* changedLayer;
        for (size_t i = 0; i < scn->layers.size(); i++)
        {
            if (scn->layers[i]->getName() == layer_name) {
                changedLayer = scn->layers[i];
                break;
            }
        }
        for (int shapeIndex : scn->pShapes)
        {
            scn->globalShapes[shapeIndex].getLayer()->deleteShape(shapeIndex);
            scn->globalShapes[shapeIndex].changeLayer(changedLayer);
            scn->globalShapes[shapeIndex].getLayer()->addShape(shapeIndex);
        }
    }
    static void OnSelectMaterial(int material_index, Project* scn)
    {
        for (size_t i = 0; i < scn->pShapes.size(); i++)
        {
            int shapeIndex = scn->pShapes[i];
            scn->SetShapeMaterial(shapeIndex, material_index);
        }
    }

    static void OnPlayChanged(bool play, Project* scn)
    {
        scn->globalTime = 0;
    }
};