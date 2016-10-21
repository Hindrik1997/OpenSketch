//
// Created by Hindrik Stegenga on 30-9-16.
//

#include "MoveShapeState.h"
#include "../Application.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "../Commands/ChangeShapeCommand.h"


void MoveShapeState::doAction(Application *_context)
{
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
                rect.setPosition(static_cast<int>(v.x + offset.x),static_cast<int>(v.y + offset.y));
                s_selectedShapeLastFrame = selected;
                s_selectedxOffset = static_cast<int>(offset.x);
                s_selectedyOffset = static_cast<int>(offset.y);
                s_original_x = static_cast<int>(rect.getPosition().x);
                s_original_y = static_cast<int>(rect.getPosition().y);

                selected->setSelected(true);
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
                selected->setPosition(static_cast<int>(v.x + s_selectedxOffset),static_cast<int>(v.y + s_selectedyOffset));
            }
            else
            {
                if(s_selectedShapeLastFrame != nullptr)
                {
                    //muis laat hier los
                    size_t index = 0;
                    for(size_t i = 0; i < _context->getGLManager().getShapes().size(); ++i)
                    {
                        if(&_context->getGLManager().getShapes()[i] == s_selectedShapeLastFrame)
                        {
                            index = i;
                            break;
                        }
                    }


                    Shape* rect = s_selectedShapeLastFrame;
                    rect->setSelected(false);

                    int currentx, currenty;
                    currentx = static_cast<int>(rect->getPosition().x);
                    currenty = static_cast<int>(rect->getPosition().y);

                    int xdiff = currentx - s_original_x;
                    int ydiff = currenty - s_original_y;

                    rect->setPosition(s_original_x, s_original_y);

                    _context->execute(new ChangeShapeCommand(index, xdiff,ydiff,0,0));
                }
                s_selectedxOffset = 0; s_selectedyOffset = 0; s_original_x = 0; s_original_y = 0;
                s_selectedShapeLastFrame = nullptr;
            }
        }
    } else {
        if(s_selectedShapeLastFrame != nullptr)
        {
            s_selectedShapeLastFrame->setSelected(false);
        }
        s_selectedxOffset = 0; s_selectedyOffset = 0; s_original_x = 0; s_original_y = 0;
        s_selectedShapeLastFrame = nullptr;
    }
}
