//
// Created by Hindrik Stegenga on 30-9-16.
//

#include "EditShapeState.h"
#include "../Application.h"
#include "../OpenGL/OpenGLRenderManager.h"

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

    shapeInfo currentSelected;
    void* a = _context->getGLManager().getSelectedRectangle();
    if(a == nullptr)
    {
        a = _context->getGLManager().getSelectedEllipse();
        if(a != nullptr) {
            currentSelected.m_shapePointer = a;
            currentSelected.m_shapeT = shapeType::EllipseType;
        } else {
            currentSelected.m_shapePointer = nullptr;
            currentSelected.m_shapeT = shapeType::NullType;
        }
    } else
    {
        currentSelected.m_shapePointer = a;
        currentSelected.m_shapeT = shapeType::RectangleType;
    }



    if(glfwGetMouseButton(_context->getM_paintWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS
       && currentSelected.m_shapePointer != nullptr
       && currentSelected.m_shapePointer != _context->getM_selectedShape().m_shapePointer)
    {
        //select shape

        //old deselecten
        if(_context->getM_selectedShape().m_shapePointer != nullptr) {

            if(_context->getM_selectedShape().m_shapeT == shapeType::RectangleType)
                static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
            else
                static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
        }

        _context->getM_selectedShape().m_shapePointer = currentSelected.m_shapePointer;
        _context->getM_selectedShape().m_shapeT = currentSelected.m_shapeT;

        if(_context->getM_selectedShape().m_shapeT == shapeType::RectangleType){
            static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSelected(true);
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), std::to_string(static_cast<int>(static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->getPosition().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), std::to_string(static_cast<int>(static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->getPosition().y)).c_str());
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), std::to_string(static_cast<int>(static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->getSize().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), std::to_string(static_cast<int>(static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->getSize().y)).c_str());
        }
        else
        {
            static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSelected(true);
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), std::to_string(static_cast<int>(static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->getPosition().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), std::to_string(static_cast<int>(static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->getPosition().y)).c_str());
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), std::to_string(static_cast<int>(static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->getSize().x)).c_str());
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), std::to_string(static_cast<int>(static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->getSize().y)).c_str());
        }
    }

    if(_context->getM_selectedShape().m_shapePointer != nullptr)
    {
        if(_context->getM_selectedShape().m_shapeT == shapeType::RectangleType){
            static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSelected(true);
        } else
        {
            static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSelected(true);
        }


        if(_context->isM_isEdited())
        {
            //get new info from tool and set

            string posx(gtk_entry_get_text(GTK_ENTRY(_context->getM_posxBox())));
            string posy(gtk_entry_get_text(GTK_ENTRY(_context->getM_posyBox())));
            string sizex(gtk_entry_get_text(GTK_ENTRY(_context->getM_sizexBox())));
            string sizey(gtk_entry_get_text(GTK_ENTRY(_context->getM_sizeyBox())));

            int px,py, sx,sy;

            try
            {
                //Throwed als de conversie faalt
                px = std::stoi(posx);
                py = std::stoi(posy);
                sx = std::stoi(sizex);
                sy = std::stoi(sizey);
            }
            catch (...)
            {
                //Niet accepten
                if(_context->getM_selectedShape().m_shapeT == shapeType::RectangleType){
                    static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
                } else
                {
                    static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
                }
                _context->getM_selectedShape().m_shapePointer = nullptr;
                _context->getM_selectedShape().m_shapeT = shapeType::NullType;
                return;
            }

            if(_context->getM_selectedShape().m_shapeT == shapeType::RectangleType){
                static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setPosition(px,py);
                static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSize(sx,sy);

                //shape stuff weer goed zetten
                static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
            }
            else
            {
                static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setPosition(px,py);
                static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSize(sx,sy);

                //shape stuff weer goed zetten
                static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
            }


            _context->getM_selectedShape().m_shapePointer = nullptr;
            _context->getM_selectedShape().m_shapeT = shapeType::NullType;
            //set edit to false
            _context->setShapeEdited(false);

            gtk_entry_set_text(GTK_ENTRY(_context->getM_posxBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_posyBox()), "selected");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizexBox()), "Nothing");
            gtk_entry_set_text(GTK_ENTRY(_context->getM_sizeyBox()), "selected");
        }

        if(_context->isM_isEdited())
        {

            if(_context->getM_selectedShape().m_shapeT == shapeType::RectangleType){
                static_cast<Rectangle*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
                vector<Rectangle>& vec = const_cast<vector<Rectangle>&>(_context->getGLManager().getRectangles());
                for( size_t i = 0; i < vec.size(); ++i)
                {
                    if(&vec[i] == _context->getM_selectedShape().m_shapePointer)
                    {
                        vec.erase(vec.begin() + i);
                        break;
                    }
                }
            } else
            {
                static_cast<Ellipse*>(_context->getM_selectedShape().m_shapePointer)->setSelected(false);
                vector<Ellipse>& vec = const_cast<vector<Ellipse>&>(_context->getGLManager().getEllipses());
                for( size_t i = 0; i < vec.size(); ++i)
                {
                    if(&vec[i] == _context->getM_selectedShape().m_shapePointer)
                    {
                        vec.erase(vec.begin() + i);
                        break;
                    }
                }
            }

            _context->getM_selectedShape().m_shapePointer = nullptr;
            _context->getM_selectedShape().m_shapeT = shapeType::NullType;
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