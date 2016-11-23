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
 * In jou visitor implementatie kan je dus zelf kiezen of je de decorator overrides called of enkel die van de base shape.
 *
 * Hier zijn drie functies voor:
 *
 * call_decorated() -> called de functie inclusief de overrides ervan in de decorators. Hier zit geen beveiliging op,
 *      dus indien er geen decorator is dan segfault ie met een nullptr dereference.
 *
 * call_non_decorated() -> called altijd de functie van de shape klasse zelf. Zonder de decorator overrides.
 *
 * call_automatic() -> indien er een override in een decorator is wordt deze gecalled,
 *      anders niet. Aangeraden wordt om deze te gebruiken.
 *
 * met call_decorated() kan je de functie callen inclusief de decorated overrides, en met call_non_decorated() puur
 * die van de class zelf. (Als deze een derived is, bijv. Group, is deze dus wel overriden zoals normaal!)
 * Indien de functie een waarde teruggeeft specificeer je deze als eerste template parameter. i.e:
 * call_automatic<int>(&Shape::getSomeInt, _shape);
 *
 * Ook is er support voor upcasts in de vorm van een 2e template parameter, op deze manier kan je ook functies callen
 * van specifiekere types van shape. Bijv. Group. i.e:
 * call_automatic<int, Group>(&Group::someFunc, _group);
 *
 *
 *  De rest van de template types worden automatisch door de compiler gesolved.
 * */

#include <deque>
#include "../includeLibs.h"
#include "../SupportClasses.h"

using std::deque;

class Shape;
class Group;
class ShapeDecorator;

class Visitor
{
private:
    deque<Shape*> m_deque;
public:
    virtual void start_visit(Shape &_shape) = 0;
    virtual void start_visit(Group &_group) = 0;

    virtual void stop_visit(Shape& _shape) = 0;
    virtual void stop_visit(Group& _group) = 0;

    virtual void start_visit(ShapeDecorator& _decorator) final;
    virtual void stop_visit(ShapeDecorator& _decorator) final;

    virtual ~Visitor() = 0;

    //nog veel meer template voodoo

    template<typename R, typename F, typename O, typename... Args>
    R call_non_decorated(F _func, O& _object, Args... _args);

    template<typename F, typename O, typename... Args>
    void call_non_decorated(F _func, O& _object, Args... _args);

    template<typename R, typename D, typename F, typename... Args>
    R call_decorated(F _func, Args... _args);

    template<typename R, typename F, typename... Args>
    R call_decorated(F _func, Args... _args);

    template<typename F, typename... Args>
    void call_decorated(F _func, Args... _args);

    template<typename R, typename F, typename O, typename... Args>
    R call_automatic(F _func,O& _object, Args... _args);

    template<typename R, typename D, typename F, typename O, typename... Args>
    R call_automatic(F _func,O& _object, Args... _args);

    template<typename F, typename O, typename...Args>
    void call_automatic(F _func,O& _object, Args... _args);

};


//Ook hier weer een implementatie, omdat deze verplicht is, zelfs als deze puur virtueel is.
inline Visitor::~Visitor(){}


//called de decorated versie van de functie
template<typename R, typename F, typename... Args>
R Visitor::call_decorated(F _func, Args... _args) {
    return (m_deque.front()->*_func)(_args...);
}

//called de decorated versie van de functie, maar cast naar type D
template<typename R, typename D, typename F, typename... Args>
R Visitor::call_decorated(F _func, Args... _args) {
    return (dynamic_cast<D*>(m_deque.front())->*_func)(_args...);
}

//void return type versie van de decorated call
template<typename F, typename... Args>
void Visitor::call_decorated(F _func, Args... _args)
{
    return (m_deque.front()->*_func)(_args...);
};

//called de non_decorated versie van de functie
template<typename R, typename F, typename O, typename... Args>
R Visitor::call_non_decorated(F _func, O& _object, Args... _args)
{
    return (_object.*_func)(_args...);
};


//void specialisatie van de non decorated versie
template<typename F, typename O, typename... Args>
void Visitor::call_non_decorated(F _func, O &_object, Args... _args) {
    return (_object.*_func)(_args...);
}


//bepaalt zelf welke hij moet callen, vereist return type
template<typename R, typename F,typename O, typename... Args>
R Visitor::call_automatic( F _func, O& _object, Args... _args) {
    if(m_deque.empty())
    {
        return (_object.*_func)(_args...);
    }
    else
    {
        return (m_deque.front()->*_func)(_args...);
    }
}

//Bepaald het ook zelf, maar voert een dynamic cast naar type D uit.
template<typename R, typename D, typename F,typename O, typename... Args>
R Visitor::call_automatic(F _func, O &_object, Args... _args) {
    if(m_deque.empty())
    {
        return (_object.*_func)(_args...);
    }
    else
    {
        return (dynamic_cast<D*>(m_deque.front())->*_func)(_args...);
    }
}

//bepaalt zelf welke hij moet callen, en returned void
template<typename F, typename O, typename...Args>
void Visitor::call_automatic(F _func, O& _object, Args... _args) {
    if(m_deque.empty())
    {
        return (_object.*_func)(_args...);
    }
    else
    {
        return (m_deque.front()->*_func)(_args...);
    }
}

#endif //OPEN_SKETCH_VISITOR_H