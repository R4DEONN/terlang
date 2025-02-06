#pragma once

#include "RegexTree.h"

#include <string_view>
#include <vector>

class DFA
{
public:
    class State
    {
    private:
        struct Transition
        {
            Transition(const State* s, char c) : state(s), character(c){}

            const State* state;
            char character;
        };

    public:
        void AddTransition(State* state, char c)
        {
            transitions.emplace_back(state, c);
        }
        bool IsAcceptState() const { return is_accept_state; }
        void MakeAcceptState() { is_accept_state = true; }
        const std::vector<Transition>& Transitions() const { return transitions; }

    private:
        std::vector<Transition> transitions;
        bool is_accept_state;
    };

    explicit DFA(const RegexTree& tree);

    void CreateDotFile(std::string_view fileName) const;

	void WriteDFA(std::string_view fileName) const;

private:
    std::vector<std::unique_ptr<State>> states;
};