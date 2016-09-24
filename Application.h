//
// Created by Hindrik Stegenga on 24-9-16.
//

#ifndef OPEN_SKETCH_APPLICATION_H
#define OPEN_SKETCH_APPLICATION_H

/*
 * SINGLETON PATTERN
 * getInstance() retourneert de instance
 * de constructor is private, zodat je die niet zomaar kan callen
 * en de copy constructor, move constructor, assignment operator en move assignment operator zijn gedelete,
 * zodat ze niet gecalled kunnen worden.
 */



class Application {
public:
    static Application& getInstance();
private:
    Application();
public:
    Application(const Application&) = delete;
    Application(const Application&&) = delete;
    void operator=(const Application&) = delete;
    void operator=(const Application&&) = delete;
private:












};


#endif //OPEN_SKETCH_APPLICATION_H
