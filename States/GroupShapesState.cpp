//
// Created by Hindrik Stegenga on 25-10-16.
//

#include "../Application.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "GroupShapesState.h"
#include <algorithm>

//returns true if clicked
bool mouse(Application* _context)
{
    static int state;

    if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if(state == GLFW_RELEASE)
        {
            state = GLFW_PRESS;
            return true;
        }
    }
    else
    {
        if(state == GLFW_PRESS)
            state = GLFW_RELEASE;
    }
    return false;
}

void GroupShapesState::doAction(Application *_context) {

    if(mouse(_context))
    {
        //get selected shape
        Shape* selected = _context->getGLManager().getSelectedShape();

        if(selected == nullptr)
        {
            //if no shape selected, clear selected shape array and set shapes selected to false
            _context->resetState();
            return;
        }

        //if shape already selected, deselect
        vector<Shape*>& vec = _context->getSelectedShapes();
        if(std::find(vec.begin(), vec.end(), selected) != vec.end())
        {
            selected->setSelected(false);

            vec.erase(std::remove(vec.begin(), vec.end(), selected), vec.end());
            return;
        }

        //else we select it and push the pointer in the vector
        vec.push_back(selected);
        selected->setSelected(true);
    }
}