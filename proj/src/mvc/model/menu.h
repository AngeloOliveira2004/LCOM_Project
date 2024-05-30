#include <lcom/lcf.h>
#include <stdio.h>
#include "string.h"

#pragma once

enum MenuStates
{
    MAIN_MENU,
    GAME_MENU,
    SETTINGS_MENU,
    
    EXIT_MENU
};


struct Menu {
    char* name;
    char* description;
    double price;
    char* category;
    enum MenuStates state;
};
