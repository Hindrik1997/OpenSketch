//
// Created by Hindrik Stegenga on 24-9-16.
//

#include "GTKWindow.h"
#include "GTKManager.h"

int GTKWindow::m_globalIDcounter = 0;

GTKWindow::GTKWindow(GTKManager& _manager, int _width, int _height, string _title)
        : m_id(m_globalIDcounter), m_Manager(_manager), m_width(_width), m_height(_height), m_title(_title)
{
    m_globalIDcounter++;

    m_pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    GdkGeometry windProps;
    windProps.min_width = m_width;
    windProps.min_height = m_height;
    gtk_window_set_geometry_hints(GTK_WINDOW(m_pWindow), NULL, &windProps, GDK_HINT_MIN_SIZE);

    gtk_window_set_resizable(GTK_WINDOW(m_pWindow), FALSE);

    gtk_window_set_title(GTK_WINDOW(m_pWindow), m_title.c_str());
}

int GTKWindow::getID()
{
    return m_id;
}

void GTKWindow::show()
{
    gtk_widget_show(m_pWindow);
}

void GTKWindow::present()
{
    gtk_window_present(GTK_WINDOW(m_pWindow));
}

void GTKWindow::hide()
{
    gtk_widget_hide(m_pWindow);
}