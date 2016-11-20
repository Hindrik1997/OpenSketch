//
// Created by Hindrik Stegenga on 30-9-16.
//

#include "EditShapeState.h"
#include "../Application.h"
#include "../OpenGL/ShapeRenderManager.h"
#include "../Commands/RemoveShapeCommand.h"
#include "../Commands/ChangeShapeCommand.h"
#include "../Commands/UnFormGroupCommand.h"
#include "../Visitors/SetSelectedVisitor.h"

void EditShapeState::doAction(Application *_context)
{
    SetSelectedVisitor vf(false);
    SetSelectedVisitor vt(true);

    Shape* currentSelected = _context->getGLManager().getSelectedShape();

    if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
       && currentSelected != nullptr
       && currentSelected != _context->getM_selectedShape())
    {
        //select shape

        //old deselecten
        if(_context->getM_selectedShape() != nullptr) {
            _context->getM_selectedShape()->accept(vf);
        }

        _context->getM_selectedShape() = currentSelected;

        _context->getM_selectedShape()->accept(vt);
        gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getPosition().x)).c_str());
        gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getPosition().y)).c_str());
        gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getSize().x)).c_str());
        gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), std::to_string(static_cast<int>(_context->getM_selectedShape()->getSize().y)).c_str());

    }

    if(_context->getM_selectedShape() != nullptr)
    {
        _context->getM_selectedShape()->accept(vt);

        if(_context->isM_isEdited()) {
            //get new info from tool and set

            string posx(gtk_entry_get_text(GTK_ENTRY(_context->getM_posxBox())));
            string posy(gtk_entry_get_text(GTK_ENTRY(_context->getM_posyBox())));
            string sizex(gtk_entry_get_text(GTK_ENTRY(_context->getM_sizexBox())));
            string sizey(gtk_entry_get_text(GTK_ENTRY(_context->getM_sizeyBox())));

            int px, py, sx, sy;

            try {
                //Throws if conversion fails, if for example user enters non number
                px = std::stoi(posx);
                py = std::stoi(posy);
                sx = std::stoi(sizex);
                sy = std::stoi(sizey);

                //numbers represent draw surface size
                if(sx <= 0 || sy <= 0 || px <= 0 || py <= 0 || sx >= PAINT_WINDOW_SIZE_X || sy >= PAINT_WINDOW_SIZE_Y || px >= PAINT_WINDOW_SIZE_X || py >= PAINT_WINDOW_SIZE_Y)
                {   //Niet accepten
                    _context->getM_selectedShape()->accept(vf);

                    _context->getM_selectedShape() = nullptr;
                    return;
                }
            } //std::stoi() throwed een invalid_arg of een out_of_range exception.
            catch (std::invalid_argument e) {
                //Niet accepten
                _context->getM_selectedShape()->accept(vf);

                _context->getM_selectedShape() = nullptr;
                return;
            }
            catch (std::out_of_range e)
            {
                //Niet accepten
                _context->getM_selectedShape()->accept(vf);

                _context->getM_selectedShape() = nullptr;
                return;
            }

            size_t indexList = _context->getGLManager().getIndex(_context->getM_selectedShape());

            Shape &rect = *_context->getM_selectedShape();

            int ox, oy, osx, osy;

            ox = static_cast<int>(rect.getPosition().x);
            oy = static_cast<int>(rect.getPosition().y);
            osx = static_cast<int>(rect.getSize().x);
            osy = static_cast<int>(rect.getSize().y);


            _context->execute(new ChangeShapeCommand(indexList, px - ox, py - oy, sx - osx, sy - osy));
            _context->getM_selectedShape()->accept(vf);


            //reset everything...

            _context->getM_selectedShape() = nullptr;

            gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), "selected");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), "selected");

            //set edit states to false
            _context->setShapeEdited(false);
            _context->setShapeDeleted(false);
            _context->setGroupDeformed(false);
        }

        if(_context->isM_isDeleted())
        {
            size_t indexList = _context->getGLManager().getIndex(_context->getM_selectedShape());
            _context->getM_selectedShape()->accept(vf);

            Shape& rect = *_context->getM_selectedShape();
            _context->execute(new RemoveShapeCommand(indexList));


            _context->getM_selectedShape() = nullptr;

            _context->setShapeEdited(false);
            _context->setShapeDeleted(false);
            _context->setGroupDeformed(false);

            gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), "selected");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), "selected");
        }

        if(_context->isM_isDegrouped())
        {
            size_t indexList = _context->getGLManager().getIndex(_context->getM_selectedShape());
            _context->getM_selectedShape()->accept(vf);

            Shape* s = _context->getM_selectedShape();

            //if it is a group:
            if(dynamic_cast<Group*>(s))
            {
                _context->execute(new UnFormGroupCommand(indexList));
            }
            _context->getM_selectedShape() = nullptr;

            _context->setShapeEdited(false);
            _context->setShapeDeleted(false);
            _context->setGroupDeformed(false);

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