//
// Created by Hindrik Stegenga on 30-9-16.
//

#ifndef OPEN_SKETCH_COMMANDCONTEXT_H
#define OPEN_SKETCH_COMMANDCONTEXT_H

#include <memory>
#include <stack>
#include "Command.h"

using std::unique_ptr;
using std::stack;

/*
 * CRTP : Curriously Recurring Template Pattern
 *
 * Door de derived classes zichzelf als template parameter mee te geven kan ik de commands toegang geven tot
 * members van de derived classes op een mooie manier, en meteen ook type-safety te garanderen.
 * */


template<typename D>
class CommandContext {
private:
    stack<unique_ptr<Command<D>>> m_history;
    stack<unique_ptr<Command<D>>> m_redoHistory;
public:
    const stack<unique_ptr<Command<D>>>& getM_history() const;
    const stack<unique_ptr<Command<D>>>& getM_redoHistory() const;
public:
    //heap allocated command!
    void execute(Command<D>* _cmd);
    void redo();
    void undo();
};

template<typename D>
const stack<unique_ptr<Command<D>>> &CommandContext<D>::getM_history() const {
    return m_history;
}

template<typename D>
const stack<unique_ptr<Command<D>>> &CommandContext<D>::getM_redoHistory() const {
    return m_redoHistory;
}

template<typename D>
void CommandContext<D>::redo()
{
    if(m_redoHistory.size() == 0)
        return;

    unique_ptr<Command<D>> cmd = std::move(m_redoHistory.top());
    m_redoHistory.pop();

    cmd->execute(static_cast<D *>(this));

    m_history.push(std::move(cmd));
}

template<typename D>
void CommandContext<D>::undo()
{
    if(m_history.size() == 0)
        return;

    unique_ptr<Command<D>> cmd = std::move(m_history.top());
    m_history.pop();

    cmd->undo(static_cast<D *>(this));

    m_redoHistory.push(std::move(cmd));
}

template<typename D>
void CommandContext<D>::execute(Command<D>* _cmd)
{
    if(_cmd == nullptr)
        throw "Please, don't be such an idiot!";

    unique_ptr<Command<D>> cmd = std::unique_ptr<Command<D>>(_cmd);
    cmd->execute(static_cast<D*>(this));
    m_history.push(std::move(cmd));
}


#endif //OPEN_SKETCH_COMMANDCONTEXT_H