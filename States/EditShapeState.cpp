//
// Created by Hindrik Stegenga on 30-9-16.
//

#include "EditShapeState.h"
#include "../Application.h"
#include "../OpenGL/OpenGLRenderManager.h"
#include "../Commands/RemoveShapeCommand.h"
#include "../Commands/ChangeShapeCommand.h"

void EditShapeState::doAction(Application *_context)
{

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

    Shape* currentSelected = _context->getGLManager().getSelectedShape();



    if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
       && currentSelected != nullptr
       && currentSelected != _context->getM_selectedShape())
    {
        //select shape

        //old deselecten
        if(_context->getM_selectedShape() != nullptr) {
            _context->getM_selectedShape()->setSelected(false);
        }

        _context->getM_selectedShape() = currentSelected;

        _context->getM_selectedShape()->setSelected(true);
        gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getPosition().x)).c_str());
        gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getPosition().y)).c_str());
        gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getSize().x)).c_str());
        gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getSize().y)).c_str());

    }

    if(_context->getM_selectedShape() != nullptr)
    {
        _context->getM_selectedShape()->setSelected(true);

        if(_context->isM_isEdited()) {
            //get new info from tool and set

            string posx(gtk_entry_get_text(GTK_ENTRY(_context->getM_posxBox())));
            string posy(gtk_entry_get_text(GTK_ENTRY(_context->getM_posyBox())));
            string sizex(gtk_entry_get_text(GTK_ENTRY(_context->getM_sizexBox())));
            string sizey(gtk_entry_get_text(GTK_ENTRY(_context->getM_sizeyBox())));

            int px, py, sx, sy;

            try {
                //Throwed als de conversie faalt
                px = std::stoi(posx);
                py = std::stoi(posy);
                sx = std::stoi(sizex);
                sy = std::stoi(sizey);
            }
            catch (...) {
                //Niet accepten
                _context->getM_selectedShape()->setSelected(false);

                _context->getM_selectedShape() = nullptr;
                return;
            }

            vector<Shape> &vec = const_cast<vector<Shape> &>(_context->getGLManager().getShapes());
            size_t index = 0;
            for (size_t i = 0; i < vec.size(); ++i) {
                if (&vec[i] == _context->getM_selectedShape()) {
                    index = i;
                }
            }

            Shape &rect = *_context->getM_selectedShape();

            int ox, oy, osx, osy;

            ox = static_cast<int>(rect.getPosition().x);
            oy = static_cast<int>(rect.getPosition().y);
            osx = static_cast<int>(rect.getSize().x);
            osy = static_cast<int>(rect.getSize().y);


            _context->execute(new ChangeShapeCommand(index, px - ox, py - oy, sx - osx, sy - osy));

            //static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setPosition(px,py);
            //static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSize(sx,sy);

            //shape stuff weer goed zetten
            _context->getM_selectedShape()->setSelected(false);

            _context->getM_selectedShape() = nullptr;
            //set edit to false
            _context->setShapeEdited(false);

            gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), "selected");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), "selected");

        }

        if(_context->isM_isDeleted())
        {

            _context->getM_selectedShape()->setSelected(false);
            vector<Shape>& vec = const_cast<vector<Shape>&>(_context->getGLManager().getShapes());
            for( size_t i = 0; i < vec.size(); ++i)
            {
                if(&vec[i] == _context->getM_selectedShape())
                {

                    Shape& rect = *_context->getM_selectedShape();
                    _context->execute(new RemoveShapeCommand(i, static_cast<int>(rect.getPosition().x), static_cast<int>(rect.getPosition().y), static_cast<int>(rect.getSize().x), static_cast<int>(rect.getSize().y),rect.getDrawer()));
                    break;
                }
            }

            _context->getM_selectedShape() = nullptr;
            _context->setShapeEdited(false);
            _context->setShapeDeleted(false);

            gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), "selected");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), "selected");
        }
    }
    else
    {
        //nothing selected
        _context->setShapeEdited(false);
        gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), "Nothing");
        gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), "selected");
        gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), "Nothing");
        gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), "selected");
    }
}