//
// Created by Hindrik Stegenga on 20-10-16.
//

#ifndef OPEN_SKETCH_RECTANGLEDRAWER_H
#define OPEN_SKETCH_RECTANGLEDRAWER_H

#include "Drawer.h"
#include "ShapeRenderManager.h"

/*
 * SINGLETON PATTERN
 * getInstance() retourneert de instance
 * de constructor is private, zodat je die niet zomaar kan callen
 * en de copy constructor, move constructor, assignment operator en move assignment operator zijn gedelete,
 * zodat ze niet gecalled kunnen worden.
 */

class RectangleDrawer : public Drawer {
public:
    static RectangleDrawer& getInstance();
private:
    RectangleDrawer();
public:
    RectangleDrawer(const RectangleDrawer&) = delete;
    RectangleDrawer& operator=(const RectangleDrawer&) = delete;

    RectangleDrawer(RectangleDrawer&&) = delete;
    RectangleDrawer& operator=(RectangleDrawer&&) = delete;
    ~RectangleDrawer() = default;
public:
    std::string toString() const;
};


#endif //OPEN_SKETCH_RECTANGLEDRAWER_H
