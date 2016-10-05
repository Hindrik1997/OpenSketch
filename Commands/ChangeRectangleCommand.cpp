//
// Created by hindrik on 5-10-16.
//

#include "ChangeRectangleCommand.h"

ChangeRectangleCommand::ChangeRectangleCommand(std::size_t _index,int _posx_change, int _posy_change, int _sizex_change, int _sizey_change)
: m_posx_change(_posx_change), m_posy_change(_posy_change), m_sizex_change(_sizex_change), m_sizey_change(_sizey_change)
{

}

void ChangeRectangleCommand::execute(Application *_context)
{

}

void ChangeRectangleCommand::undo(Application *_context)
{

}