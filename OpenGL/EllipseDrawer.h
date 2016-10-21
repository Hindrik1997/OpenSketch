//
// Created by Hindrik Stegenga on 20-10-16.
//

#ifndef OPEN_SKETCH_ELLIPSEDRAWER_H
#define OPEN_SKETCH_ELLIPSEDRAWER_H

#include "Drawer.h"
#include "OpenGLRenderManager.h"

/*
 * SINGLETON PATTERN
 * getInstance() retourneert de instance
 * de constructor is private, zodat je die niet zomaar kan callen
 * en de copy constructor, move constructor, assignment operator en move assignment operator zijn gedelete,
 * zodat ze niet gecalled kunnen worden.
 */

class EllipseDrawer : public Drawer {
public:
    static EllipseDrawer& getInstance();
private:
    EllipseDrawer();
public:
    EllipseDrawer(const EllipseDrawer&) = delete;
    EllipseDrawer& operator=(const EllipseDrawer&) = delete;

    EllipseDrawer(EllipseDrawer&&) = delete;
    EllipseDrawer& operator=(EllipseDrawer&&) = delete;
    ~EllipseDrawer() = default;
public:
    std::string toString() const;
};


#endif //OPEN_SKETCH_ELLIPSEDRAWER_H
