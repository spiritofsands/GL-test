#include <stdio.h>

#define LIST_OF_STATES \
  STATE(off) \
  STATE(on) \
  STATE(espresso_ready) \
  STATE(make_espresso) \
  STATE(steamer_ready) \
  STATE(make_steam) \
  STATE(drink_ready) \
  STATE(error)

#define STATE(name) int name##_state();
LIST_OF_STATES
#undef STATE

int (* state[])() = {
#define STATE(name) name##_state,
LIST_OF_STATES
#undef STATE
};

enum state_codes {
#define STATE(name) name,
LIST_OF_STATES
#undef STATE
};

enum action_codes {
  switch_on,
  switch_off,
  heat_up,
  espresso_on,
  espresso_off,
  steam_on,
  steam_off,
  steam_knob_forward,
  steam_knob_backward,
  end
};

struct transition {
  enum state_codes current_state;
  enum action_codes action_code;
  enum state_codes next_state;
};

/* 2D table can be used for O(1) switching */
struct transition state_transitions[] = {
  {off,            switch_on,           on},
  {on,             switch_off,          off},
  {on,             heat_up,             espresso_ready},
  {espresso_ready, switch_off,          off},
  {espresso_ready, steam_on,            steamer_ready},
  {espresso_ready, espresso_on,         make_espresso},
  {steamer_ready,  switch_off,          off},
  {steamer_ready,  steam_off,           espresso_ready},
  {steamer_ready,  steam_knob_forward,  make_steam},
  {make_steam,     steam_knob_backward, steamer_ready},
  {make_steam,     end,                 drink_ready},
  {make_espresso,  espresso_off,        espresso_ready},
  {make_espresso,  end,                 drink_ready}
};

enum state_codes make_transition(enum state_codes, enum action_codes);

int main(int argc, char **argv) {
  enum state_codes current_state = off;
  enum state_codes end_state = drink_ready;
  enum state_codes error_state = error;

  enum action_codes action;
  int (* state_function)(void);

  for (;;) {
    state_function = state[current_state];
    action = state_function();

    if (current_state == end_state || current_state == error_state)
      break;

    current_state = make_transition(current_state, action);
  }

  return 0;
}

enum state_codes make_transition(enum state_codes current_state, enum action_codes action) {
  unsigned int length = sizeof(state_transitions) / sizeof(state_transitions[0]);
  for (unsigned int i = 0; i < length; ++i) {
    if (state_transitions[i].current_state == current_state &&
        state_transitions[i].action_code == action) {
      return state_transitions[i].next_state;
    }
  }

  return error;
}

/*
 *
 * Scenario
 *
 */

int error_state() {
  printf("%s", "Can't find next state, emergency shutdown\n");
}

int off_state() {
  printf("%s", "Off\n");

  return switch_on;
}

int on_state() {
  printf("%s", "On\n");

  return heat_up;
}

int espresso_ready_state() {
  printf("%s", "Ready to make espresso\n");

  return espresso_on;
}

int make_espresso_state() {
  printf("%s", "Making espresso\n");

  return end;
}

int steamer_ready_state() {
  printf("%s", "Ready to make some steam\n");

  return steam_knob_forward;
}

int make_steam_state() {
  printf("%s", "Making steam\n");

  return end;
}

int drink_ready_state() {
  printf("%s", "Drink is ready\n");
}
