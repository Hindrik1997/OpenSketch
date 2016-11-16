//
// Created by Hindrik Stegenga on 30-9-16.
//

#include "MoveShapeState.h"
#include "../Application.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "../Commands/ChangeShapeCommand.h"
#include "../Visitors/MoveShapeVisitor.h"
#include "../Visitors/SetSelectedVisitor.h"


void MoveShapeState::doAction(Application *_context)
{

    SetSelectedVisitor vt(true);
    SetSelectedVisitor vf(false);

    _context->resetState();

    int w,h;
    _context->getPaintWindowSize(w,h);
    glm::vec2 v(static_cast<int>(_context->getPaintWindowCursorPos().x),
                static_cast<int>(_context->getPaintWindowCursorPos().y));
    if(v.x < 0)
        v.x = 0;
    if(v.x > w)
        v.x = w;
    if(v.y < 0)
        v.y = 0;
    if(v.y > h)
        v.y = h;

    static Shape* s_selectedShapeLastFrame = nullptr;
    static int s_selectedxOffset = 0, s_selectedyOffset = 0, s_original_x = 0, s_original_y = 0;

    Shape* selected;
    if(s_selectedShapeLastFrame != nullptr)
    {
        selected = _context->getGLManager().getSelectedShapePriority(s_selectedShapeLastFrame);
    }
    else
    {
        selected = _context->getGLManager().getSelectedShape();
    }


    if(selected != nullptr)
    {
        if(s_selectedShapeLastFrame == nullptr)
        {
            //new triangle, no last triangle or not the same
            glm::vec2 offset;
            if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                offset = _context->getGLManager().getMouseOffsetInShape(*selected, static_cast<int>(v.x), static_cast<int>(v.y));
                Shape& rect = *selected;

                int xdiff = static_cast<int>(rect.getPosition().x - (v.x + offset.x));
                int ydiff = static_cast<int>(rect.getPosition().y - (v.y + offset.y));


                MoveShapeVisitor v2(xdiff, ydiff);
                rect.accept(v2);

                s_selectedShapeLastFrame = selected;
                s_selectedxOffset = static_cast<int>(offset.x);
                s_selectedyOffset = static_cast<int>(offset.y);
                s_original_x = static_cast<int>(rect.getPosition().x);
                s_original_y = static_cast<int>(rect.getPosition().y);

                selected->accept(vt);
            }
            else
            {
                s_selectedxOffset = 0; s_selectedyOffset = 0; s_selectedShapeLastFrame = nullptr;
                s_original_x = 0; s_original_y = 0;
            }
        }
        else
        {
            //same shape
            if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                int xdiff = static_cast<int>(v.x + s_selectedxOffset) - static_cast<int>(selected->getPosition().x);
                int ydiff = static_cast<int>(v.y + s_selectedyOffset) - static_cast<int>(selected->getPosition().y);

                MoveShapeVisitor v3(xdiff, ydiff);
                selected->accept(v3);
            }
            else
            {
                if(s_selectedShapeLastFrame != nullptr)
                {
                    //muis laat hier los

                    size_t indexList = _context->getGLManager().getIndex(s_selectedShapeLastFrame);

                    Shape* rect = s_selectedShapeLastFrame;
                    rect->accept(vf);

                    int currentx, currenty;
                    currentx = static_cast<int>(rect->getPosition().x);
                    currenty = static_cast<int>(rect->getPosition().y);

                    int xdiff = currentx - s_original_x;
                    int ydiff = currenty - s_original_y;

                    MoveShapeVisitor v4(-xdiff,-ydiff);
                    rect->accept(v4);

                    _context->execute(new ChangeShapeCommand(indexList, xdiff,ydiff,0,0));
                }
                s_selectedxOffset = 0; s_selectedyOffset = 0; s_original_x = 0; s_original_y = 0;
                s_selectedShapeLastFrame = nullptr;
            }
        }
    } else {
        if(s_selectedShapeLastFrame != nullptr)
        {
            s_selectedShapeLastFrame->accept(vf);
        }
        s_selectedxOffset = 0; s_selectedyOffset = 0; s_original_x = 0; s_original_y = 0;
        s_selectedShapeLastFrame = nullptr;
    }
}