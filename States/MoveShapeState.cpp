//
// Created by Hindrik Stegenga on 30-9-16.
//

#include "MoveShapeState.h"
#include "../Application.h"
#include "../OpenGL/OpenGLRenderManager.h"


void MoveShapeState::doAction(Application *_context)
{
    _context->resetState();

    if(_context->getM_selectedShape().m_shapePointer != nullptr)
    {
        if(_context->getM_selectedShape().m_shapeT == shapeType::RectangleType)
        {
            static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
        } else
        {
            static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
        }
    }
    _context->getM_selectedShape().m_shapePointer = nullptr;
    _context->getM_selectedShape().m_shapeT = shapeType::NullType;




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




    static shapeInfo s_selectedShapeLastFrame { nullptr, shapeType::NullType };
    static int s_selectedxOffset = 0, s_selectedyOffset = 0;



    shapeInfo selected;
    if(s_selectedShapeLastFrame.m_shapePointer != nullptr)
    {
        if(s_selectedShapeLastFrame.m_shapeT == shapeType::RectangleType)
        {
            selected.m_shapePointer = _context->getGLManager().getSelectedRectanglePriority(static_cast<Rectangle*>(s_selectedShapeLastFrame.m_shapePointer));
            selected.m_shapeT = shapeType::RectangleType;
        }
        else
        {
            selected.m_shapePointer = _context->getGLManager().getSelectedEllipsePriority(static_cast<Ellipse*>(s_selectedShapeLastFrame.m_shapePointer));
            selected.m_shapeT = shapeType::EllipseType;
        }
    }
    else
    {
        selected.m_shapePointer = _context->getGLManager().getSelectedRectangle();
        selected.m_shapeT = shapeType::RectangleType;
        if(selected.m_shapePointer == nullptr)
        {
            selected.m_shapePointer = _context->getGLManager().getSelectedEllipse();
            if(selected.m_shapePointer != nullptr)
                selected.m_shapeT = shapeType::EllipseType;
        }
    }


    if(selected.m_shapePointer != nullptr)
    {
        if(s_selectedShapeLastFrame.m_shapePointer == nullptr)
        {
            //new triangle, no last triangle or not the same
            glm::vec2 offset;
            if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                if(selected.m_shapeT == shapeType::RectangleType)
                {
                    offset = _context->getGLManager().getMouseOffsetInRectangle(*static_cast<Rectangle*>(selected.m_shapePointer), static_cast<int>(v.x), static_cast<int>(v.y));
                    static_cast<Rectangle*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + offset.x),static_cast<int>(v.y + offset.y));
                    s_selectedShapeLastFrame.m_shapePointer = selected.m_shapePointer;
                    s_selectedShapeLastFrame.m_shapeT = selected.m_shapeT;
                    s_selectedxOffset = static_cast<int>(offset.x);
                    s_selectedyOffset = static_cast<int>(offset.y);
                    static_cast<Rectangle*>(selected.m_shapePointer)->setSelected(true);
                } else
                {
                    offset = _context->getGLManager().getMouseOffsetInEllipse(*static_cast<Ellipse*>(selected.m_shapePointer), static_cast<int>(v.x), static_cast<int>(v.y));
                    static_cast<Ellipse*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + offset.x),static_cast<int>(v.y + offset.y));
                    s_selectedShapeLastFrame.m_shapePointer = selected.m_shapePointer;
                    s_selectedShapeLastFrame.m_shapeT = selected.m_shapeT;
                    s_selectedxOffset = static_cast<int>(offset.x);
                    s_selectedyOffset = static_cast<int>(offset.y);
                    static_cast<Ellipse*>(selected.m_shapePointer)->setSelected(true);
                }
            }
            else
            {
                s_selectedxOffset = 0; s_selectedyOffset = 0; s_selectedShapeLastFrame.m_shapePointer = nullptr; s_selectedShapeLastFrame.m_shapeT = shapeType::NullType;
            }
        }
        else
        {
            //same shape
            if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            {
                if(selected.m_shapeT == shapeType::RectangleType)
                    static_cast<Rectangle*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + s_selectedxOffset),static_cast<int>(v.y + s_selectedyOffset));
                else
                    static_cast<Ellipse*>(selected.m_shapePointer)->setPosition(static_cast<int>(v.x + s_selectedxOffset),static_cast<int>(v.y + s_selectedyOffset));
            }
            else
            {
                if(s_selectedShapeLastFrame.m_shapePointer != nullptr)
                {
                    if(s_selectedShapeLastFrame.m_shapeT == shapeType::RectangleType)
                        static_cast<Rectangle*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);
                    else
                        static_cast<Ellipse*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);;
                }
                s_selectedxOffset = 0; s_selectedyOffset = 0; s_selectedShapeLastFrame.m_shapePointer = nullptr; s_selectedShapeLastFrame.m_shapeT = shapeType::NullType;
            }
        }
    } else {
        if(s_selectedShapeLastFrame.m_shapePointer != nullptr)
        {
            if(s_selectedShapeLastFrame.m_shapeT == shapeType::RectangleType)
                static_cast<Rectangle*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);
            else
                static_cast<Ellipse*>(s_selectedShapeLastFrame.m_shapePointer)->setSelected(false);

        }
        s_selectedxOffset = 0; s_selectedyOffset = 0; s_selectedShapeLastFrame.m_shapePointer = nullptr; s_selectedShapeLastFrame.m_shapeT = shapeType::NullType;
    }
}
