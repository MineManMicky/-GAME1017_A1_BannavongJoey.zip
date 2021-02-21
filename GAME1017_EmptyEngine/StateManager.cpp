#include "StateManager.h"

void StateManager::Update() // Invokes Update() of the current state
{
	s_stateChange = false;
	if (!s_states.empty())
		s_states.back()->Update(); // s_states.back() represents the current state, by pointer
}

void StateManager::Render() // Invokes Render() of the current state
{
	if (!s_states.empty())
		s_states.back()->Render();
}

void StateManager::PushState(State* pState) // Invoked going to PauseState from GameState
{
	s_states.push_back(pState);
	s_states.back()->Enter();
}

void StateManager::PopState() // Invoked going back to GameState from PauseState
{
	if (s_states.size() <= 1) return; // If only one state in vector, return
	if (!s_states.empty())
	{
		s_states.back()->Exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();
	}
	s_states.back()->Resume();
}

void StateManager::ChangeState(State* pState)
{
	s_stateChange = true;
	if (!s_states.empty())
	{
		s_states.back()->Exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();
	}
	pState->Enter();
	s_states.push_back(pState);

}

void StateManager::Quit()
{
	while (!s_states.empty())
	{
		s_states.back()->Exit();
		delete s_states.back();
		s_states.back() = nullptr;
		s_states.pop_back();
	}
}

std::vector<State*>& StateManager::GetStates() { return s_states; }
std::vector<State*> StateManager::s_states;

bool StateManager::s_stateChange = false;
const bool StateManager::StateChanging() { return s_stateChange; }
