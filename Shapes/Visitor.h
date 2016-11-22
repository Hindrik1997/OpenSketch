//
// Created by Hindrik Stegenga on 5-11-16.
//

#ifndef OPEN_SKETCH_VISITOR_H
#define OPEN_SKETCH_VISITOR_H

/*
 * VISITOR BASE CLASS
 *  Base class voor visitor implementaties.
 *  start_visit() en stop_visit() moeten in elke implementatie ge-overide worden. Ookal laat je deze leeg, override ze!
 *  (Je kan niet anders in c++, omdat de klasse anders abstract is, maar ik zeg het er maar ff bij)
 */

/*
 * Ik heb deze visitor class support gegeven voor het callen van decorated versies van de functies van de class.
 * Zie ook de ShapeDecorator header file.
 * In feite simuleert het virtuele functies, dus je kan nu ook gedecoreerde versies callen in de visitor implementatie.
 * Het mooiste is nog wel dat het onafhankelijk is van de decorator of visitor implementatie.
 * De base Visitor class en de base ShapeDecorator class managen dit samen volledig onzichtbaar.
 *
 * met call_decorated() kan je de functie callen inclusief de decorated overloads, en met call_non_decorated() puur
 * die van de class zelf. (Als deze een derived is, bijv. Group, is deze dus wel overriden zoals normaal!)
 *
 * */


#include <deque>

using std::deque;

class Shape;
class Group;
class ShapeDecorator;

class Visitor
{
protected:
    deque<Shape*> m_deque;
public:
    virtual void start_visit(Shape &_shape) = 0;
    virtual void start_visit(Group &_group) = 0;

    virtual void stop_visit(Shape& _shape) = 0;
    virtual void stop_visit(Group& _group) = 0;

    virtual void start_visit(ShapeDecorator& _decorator);
    virtual void stop_visit(ShapeDecorator& _decorator);

    template<typename O, typename F, typename... Args>
    inline void call_non_decorated(O& _object ,F _func, Args... _args);

    template<typename F, typename... Args>
    inline void call_decorated(F _func, Args... _args);

    template<typename O, typename F, typename... Args>
    inline void call_automatic(O& _object, F _func, Args... args);

    virtual ~Visitor() = 0;
};

//Ook hier weer een implementatie, omdat deze verplicht is, zelfs als deze puur virtueel is.
inline Visitor::~Visitor(){}


//called de niet gedecoreerde versie van de functie
template<typename O, typename F, typename... Args>
inline void Visitor::call_non_decorated(O& _object, F _func, Args... _args) {
    (_object.*_func)(_args...);
}

//called de gedecoreerde versie van de functie
template<typename F, typename... Args>
inline void Visitor::call_decorated(F _func, Args... _args) {
    ((*m_deque.front()).*_func)(_args...);
}

//bepaalt zelf welke hij moet callen
template<typename O, typename F, typename... Args>
void Visitor::call_automatic(O &_object, F _func, Args... args) {

    if(m_deque.size() == 0)
    {
        call_non_decorated(_object, _func, args...);
    }
    else
    {
        call_decorated(_func, args...);
    }
}

#endif //OPEN_SKETCH_VISITOR_H