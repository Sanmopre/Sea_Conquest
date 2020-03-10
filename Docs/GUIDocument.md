# GUI Document

This document explains how the User Interface will be displayed in our game. Each menu has a diagram that shows how every option works and what will happen. There will be references on what we will try to emulate. (This won't be the final concepts for the game)

## Index

- [Main Menu](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUIDocument.md#main-menu)
- [In-Game Menu](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUIDocument.md#in-game-menu)
- [HUD](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUIDocument.md#hud)

### Main Menu

This will be the first screen of the game with user interface elements. 
This image is what we will base from to do our concepts for the game.
![](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUI/Main%20Menu%20Reference.jpg)

*Reference from Age of Sail*

There will be three options:

**1. Play:** There will be two buttons, "new game" and "continue", the player will be able to either start a new game or continue from his last save.

**2. Settings:** This option will show a new menu where the player will be able to tweak volumes, enable the FPS cap or set the game to fullscreen or not.

**3. Exit:** This option will close the game.

![](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUI/Main%20Menu%20Diagram.jpg)

*Main Menu diagram.*

### In-Game Menu

While playing, the player will have the option to pause the game by pressing the Escape button. The game will frezze and this menu will pop up. It is very similar to the main menu, but it won't change the screen, the menu will appear and disappear in the middle of the window.

![](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUI/In-Game%20Menu%20Reference.png)

*Reference from Civilization VI*

There will be 3 options:

**1. Resume:** This option will unpause the game.

**2. Settings:** This option will show a new menu where the player will be able to tweak volumes, enable the FPS cap or set the game to fullscreen or not.

**3. Main Menu:** This option will take the player back to the main menu.

![](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUI/In-Game%20Menu%20Diagram.png)

*In-Game Menu diagram*

### HUD

The HUD is a very important part of the UI of our game because it has to be very clear, give the most information possible and at the same time not bother the player while playing. So we're going to follow this guidelines in order to get a great HUD.

**1. Location:** The HUD is going to be located in the margins of the screen because we dont want to hide anything from the player's view. And larger icons like a mini-map will be placed on corners because we tend to avoid that information and look in the center.

**2. Order:** Everything must be grouped in different sections, resources, life/xp bars, maps... If the player knows where to find different information we're going to make things much easier for him.

**3.Scale:** As we said before, we don't want to block the vision of the player so we must create a HUD big enough so it's clear and easy to read, but small in order to get the most out of our window size. 

![](https://github.com/Sanmopre/DOLIME-CORP-PROJECT-II/blob/master/Docs/GUI/Age%20Of%20Empires%20Reference.jpg)

*Reference from Age of Empires 2*

In this example we can see how everything we mentioned is very important for the HUD in RTS games. All the information is displayed in the margins, it's simple and clear, the minimap is located in the bottom-right corner so it doesn't block anything from view and most importantly, the player has a lot of vision over the game itself.
