//
// Created by Hindrik Stegenga on 23-11-16.
//

#include "AddDecoratorCommand.h"
#include "../Shapes/Shape.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "../Decorators/OrnamentBottomDecorator.h"
#include "../Decorators/OrnamentLeftDecorator.h"
#include "../Decorators/OrnamentRightDecorator.h"
#include "../Decorators/OrnamentTopDecorator.h"

AddDecoratorCommand::AddDecoratorCommand(size_t _index, string _text, string _type) : m_index(_index), m_text(_text), m_type(_type) {
}

void AddDecoratorCommand::execute(Application *_context) {
    unique_ptr<Shape>& rect = _context->getGLManager().getShapes()[m_index];

    Shape* originalShape = rect.release();

    if(m_type == std::string("top"))
    {
        rect.reset(new OrnamentTopDecorator(m_text,originalShape));
    }
    if(m_type == std::string("bottom"))
    {
        rect.reset(new OrnamentBottomDecorator(m_text,originalShape));
    }
    if(m_type == std::string("left"))
    {
        rect.reset(new OrnamentLeftDecorator(m_text,originalShape));
    }
    if(m_type == std::string("right"))
    {
        rect.reset(new OrnamentRightDecorator(m_text,originalShape));
    }
}

void AddDecoratorCommand::undo(Application *_context) {
    unique_ptr<Shape>& rect = _context->getGLManager().getShapes()[m_index];

    Shape* originalShape = rect.release();
    ShapeDecorator* decorator = dynamic_cast<ShapeDecorator*>(originalShape);

    Shape* InternalShape = decorator->getPointer();
    rect.reset(InternalShape);
    decorator->setPointer(nullptr);
    delete decorator;
}