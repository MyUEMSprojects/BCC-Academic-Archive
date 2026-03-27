# author: Felipe Echeverria Vilhalva
# date: 03/19/2024

# function to create the transitions dictionary using tuples
def transition(alphabet, states):
    transition_dict = {}

    # convert the string into a list of tuples
    state_pairs = [(states[i:i+2], states[i:i+2]) for i in range(0, len(states), 2)]
    
    # dictionary formation
    for state_key, _ in state_pairs:
        for alphabet_key in alphabet:
            print(f"{state_key}")
            states_value_of = input(f"({state_key}, {alphabet_key}): ").replace(",", "").replace(" ", "")
            tuple_states_value_of = [states_value_of[i:i+2] for i in range(0, len(states_value_of), 2)]
            transition_dict[(state_key, alphabet_key)] = tuple(tuple_states_value_of)
    
    
    return transition_dict

# recursive function afn
def afn(word, current_states, transition_states):
    # If the word is empty, we check whether any current state is final
    if not word:
        return any(state in FINAL_STATES for state in current_states)
    
    next_states = set();
    
    for state in current_states:
        if(state, word[0]) in transition_states:
            next_states.update(transition_states[state, word[0]])
    
    # if there are no more possible states the word is no longer accepted
    if not next_states:
        return False
    
    # recursively checks the remaining word from the next states
    return afn(word[1:], next_states, transition_states)

# inputs
ALPHABET = input("input the alphabet: ").replace(",", "").replace(" ", "")
STATES = input("input the states: ").replace(",", "").replace(" ", "")
INITIAL_STATE = input("input the initial state: ").replace(",", "").replace(" ", "")
FINAL_STATES = input("input the final states: ").replace(",", "").replace(" ", "")
WORD = input("input the word to be checked: ").replace(",", "").replace(" ", "")

# Generates the transitions dictionary
TRANSITION_STATES = transition(ALPHABET, STATES)

if afn(WORD, {INITIAL_STATE}, TRANSITION_STATES):
    print(f"the word '{WORD}' is accept by AFN.")
else:
    print(f"the word '{WORD}' is not accept by AFN.")