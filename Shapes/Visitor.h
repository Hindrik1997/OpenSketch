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

//Template voodoo
template <typename F>
struct return_type;

template <typename R, typename... A>
struct return_type<R (*)(A...)>
{
    typedef R type;
};

template <typename R, typename... A>
struct return_type<R (*)(A..., ...)>
{
    typedef R type;
};

#include <deque>

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

    template<typename F, typename O, typename... Args>
    typename return_type<F>::type call_non_decorated(F _func, O& _object, Args... _args);

    template<typename F, typename O, typename... Args>
    void call_non_decorated(F _func, O& _object, Args... _args);


    template<typename F, typename... Args>
    typename return_type<F>::type call_decorated(F _func, Args... _args);

    template<typename F, typename... Args>
    void call_decorated(F _func, Args... _args);


    template<typename F, typename O, typename... Args>
    typename return_type<F>::type call_automatic(F _func,O& _object, Args... _args);

    template<typename F, typename O, typename...Args>
    void call_automatic(F _func,O& _object, Args... _args);
};


//Ook hier weer een implementatie, omdat deze verplicht is, zelfs als deze puur virtueel is.
inline Visitor::~Visitor(){}


//called de decorated versie van de functie
template<typename F, typename... Args>
typename return_type<F>::type Visitor::call_decorated(F _func, Args... _args) {
    return (m_deque.front()->*_func)(_args...);
}

//void specialisatie van de decorated call
template<typename F, typename... Args>
void Visitor::call_decorated(F _func, Args... _args)
{
    return (m_deque.front()->*_func)(_args...);
};

//called de non_decorated versie van de functie
template<typename F, typename O, typename... Args>
typename return_type<F>::type Visitor::call_non_decorated(F _func, O& _object, Args... _args)
{
    return (_object.*_func)(_args...);
};

//void specialisatie van de non decorated versie
template<typename F, typename O, typename... Args>
void Visitor::call_non_decorated(F _func, O &_object, Args... _args) {
    return (_object.*_func)(_args...);
}


//bepaalt zelf welke hij moet callen
template<typename F, typename O, typename... Args>
typename return_type<F>::type Visitor::call_automatic(F _func, O& _object, Args... _args) {
    if(m_deque.empty())
    {
        return (_object.*_func)(_args...);
    }
    else
    {
        return (m_deque.front()->*_func)(_args...);
    }
}

//bepaalt zelf welke hij moet callen met specifieke specialization voor fptr's die void returnen (void is geen template type)
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