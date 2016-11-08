//
// Created by Hindrik Stegenga on 25-10-16.
//

#include "../Application.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "GroupShapesState.h"
#include "../Visitors/SetSelectedVisitor.h"
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

        SetSelectedVisitor vFalse(false);
        SetSelectedVisitor vTrue(true);

        //if shape already selected, deselect
        vector<size_t>& vec = _context->getSelectedShapes();
        if(std::find(vec.begin(), vec.end(), _context->getGLManager().getIndex(selected)) != vec.end())
        {
            selected->accept(vFalse);

            vec.erase(std::remove(vec.begin(), vec.end(), _context->getGLManager().getIndex(selected)), vec.end());
            return;
        }

        //else we select it and push the index in the vector
        vec.push_back(_context->getGLManager().getIndex(selected));
        selected->accept(vTrue);
    }
}
