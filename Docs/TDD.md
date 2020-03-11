# Tech Design Document:
 
## Introduction
 
This is the Tech Design document for Sea Conquest. This project is being developed using C++ and SDL.
 
## Technical goals
 
Randomly generated map
2D isometric world
Command of troops
RTS battle system
 
## Target Platform
 
Hardware Requirements
 
CPU Intel Core i7-2720QM @ 3.60 GHz ( 4 cores )
GPU nVidia 4200M
RAM 8192 MB
Free disk space 200 MB
SO  Windows 10
 
## External Tools to develop
 
We will use Visual Studio Community 2019 v16.3.5 and Tiled.
 
## Branching
 
...
 
## Code Style Guidelines
1. Naming Conventions
All names should be written in English.
Use Get/Set to get or set variables.
Names must be clear and shouldn’t have double meaning.
 
Indentation:
 
if (open)
    Close()
while (open)
{
	Something();
	SomethingElse();
}
 
Variables: must all in lowercase, variable names with more than a word will separate them with an underscore.
 
Single name:
speed
 
Compound name:
camera_speed
 
Temporal variables (only used in a few lines of code) can have short/abbreviated names, whilst long-term variables (used multiple times along the code) should have explanatory and non-abbreviated names.
 
Temporal variable:
av
 
Long-term variable:
angular_velociy
 
Generic variables: should have the same name as their type, but the specifics can be excluded.
 
void AddEntity(Entity_Type type)
 
When creating an enum it must be declared class.
Enums Class: Must be all lowercase, capitalizing the first letter of each word and in singular.
 
EntityType
 
Enums Class items: All in uppercase and separating compound names with an underscore. 
 
Single name:
NORTH
 
Compound name:
NORTH_WEST
 
When creating a variable of the type enum, call it as the enum.
 
Orientation orientation;
 
When referring to an enum class item, it’s required to write the enum class name, a double colon and then the name of the item.
 
orientation = Orientation::WEST
 
Constants: Must be all uppercase using underscore to separate words.
 
MAX_COLLIDERS
 
Functions and methods:
 
Must describe the function/method task shortly and must be in lowercase capitalizing the first letter of each word. 
 
AddEntity()
 
The function name must not separate words with underscores and it also shouldn’t reference the object they belong.
 
entity_manager.AddEntity();
 
Names representing a collection of objects must be in plural
 
std::vector<Entity*> entities;
 
Boolean variables should be name as a verb
 
bool selected, attacking;
 
File content must be kept within 160 columns and semicolons determine the end of a line there should not be more code in the same line after a semicolon, exceptions apply when declaring a simple function/method
 
If exceeding the 160 columns you can break:
-After a comma
-After an operator, lining the new line with the beginning of the   expression on the previous line.
 
for (std::vector<Entity*>::iterator entity = entities.begin(); 
entity != entities.end(); entity++)
{
	...
}
 
if (select_area.x < (*entity)->position.x + (*entity)->rect.w &&
    select_area.x + select_area.w >(*entity)->position.x && 
    select_area.y < (*entity)->position.y + (*entity)->rect.h && 
    select_area.h + select_area.y >(*entity)->position.y)
{
	...
}
 
Exeption example:
GetFrame() { return current_frame; }
 
Includes:
 
Should be grouped by their hierarchical position in the system with low levels files first. Dividing lowlevel headers and the main module header file from the other module headers with a space.
 
#include <iostream>
#include "SDL\include\SDL_rect.h"
#include “j1Player.h”
 
#include “j1Window.h”
#include “j1Render.h”
 
Iterator variables:
If the variable is a number, use i, (and j, k for nested loops):
 
for (int i = 0; i < points; ++i)
for (int j = 0; j < points; ++j)
 
Else, use the standard naming convention:
 
for (std::vector<Entity*>::iterator entity = entities.begin(); 
entity != entities.end(); entity++)
 
Number variables:
 
If increasing/decreasing a numeral variable by 1 use ++/-- operators
 
counter++;
 
Whitespace:
Use space to improve readability. Add space between operators and operands, commas, and semicolons, and colons. Add space on the outside of parentheses and brackets, Visual Studio distributes the spaces this way when closing a line with a semicolon.
 
a = (b + c) * d
while (true)
doSomething(a, b, c, d)
for (i = 0; i < 10; i++)
Same applies to functions/methods and initialization of variables
App->render->AddBlitEvent(2, texture, x, y, { 0, 0, 20, 20 });
SDL_Rect rect{ 1, 1, 1, 1 }
