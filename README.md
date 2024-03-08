A project in the object-oriented programming and game development course - "Zuma's Revenge"
Presenters: Daniel Hasin 316558766, Eyal Makdoshi 208879718

General explanation of the game:
In the program the game "Zuma's Revenge" was realized
On the form screen the "menu" will open in which the following 3 options will be displayed:
Play - the game will start from the first step.
The game can be started by pressing the button with the mouse (left button) or by pressing enter on the keyboard.
How to play - a help window will be displayed to the user with a general explanation of the game - how to control the character, what is its purpose.
The window will be displayed by clicking on the button with the mouse (left button).
Exit - The program ends and the game closes.
You can close the game by pressing the button with the mouse (left button) or by pressing Esc on the keyboard.

What is the game: The game will have 10 stages where the goal of each stage is to 'clean' using the frog all the balls on the track before they reach the finish hole, within the amount of beats per player. Also the goal of the player is to score the highest number of points. It is possible to switch between the current ball and the next ball by pressing the right mouse button.
When the player has achieved the desired total of points can pass the stage, the balls will stop coming and the player will have to finish blowing up the balls that are still in the track.
Exploding all the balls while moving to the next level.
Ball reaching the finish hole lowered the player's life count by 1 and repeated the stage from the beginning.
Endgame Success: Win the entire game. Failure: The player's life total is over.

General explanation of the construction of the program:
At first the program will run the "menu" screen. When a user clicks on the "Play" button, the program will read from the recording the location of the objects, frog, hole and track, and place them in a file accordingly.
After that, the program will run in a loop until the end of reading from the input file, that is, until we have finished all the steps or disqualification. In any case, the game window will close and the menu window will be displayed again.
In each action, the program will put balls into the track with a random color (the frog is according to the user's response), check if there was a collision and handle it. Also, you will check if the step is over (successfully or unsuccessfully).
The run of each stage will end when the program detects more balls or they are for the finishing hole. information and the player is eliminated but has life left, the program will rerun the current stage.

Project High School:
Button - inherits all types of buttons - Play, How to play, Exit.
BaseObject – inherits all objects in the game, both static and dynamic.
DynamicObject: inherited from frog, ball
StaticObject - bequeathed to road, hole

The list of files we created:
Menu - Department controls the management of the menu in the game. Contains a background image as well as a vector style button, when clicking the "Start" button the game will start.
Button - an abstract class that creates a button that will be shown to us in the game menu. Holds 2 images, one normal and one highlighted that is intended to highlight the button if the mouse moves over it.
Play - a class that inherits from the Button class that implements a virtual function of its parent class Button, a response to pressing the Play button, which starts the game.
Help - a class which inherits from the Button class which serves a virtual function of its parent class Button, a response to clicking the help button, which displays a help screen to the user.
Exit - a class that inherits from the Button class that implements a virtual function of its parent class Button, a response to pressing the exit button, which ends the program.
Controller - responsible for managing the program and running its forms loop. Holds a certain type of screen.
Screen - the department responsible for stage management game. It contains the stage window as well as: an object of type Frog, Hole, 2 vectors of Ball style objects, one for the balls in the track and one for the balls fired by the user, a vector of objects of type Road, a vector of coordinates that will contain the road nodes of the balls, a vector that will contain In addition, it will contain variables that update the phase number, a single clock.
BaseObject – abstract class. The base class for all objects that exist between them.
DynamicObject – an abstract class that inherits from the base class BaseObject and the base class for all mobile objects in the game.
Resources - the Singleton class which contains all the resources we use in the program, meaning texture type objects for each of the characters (static and dynamic) and animations, as well as fonts and sounds that the program has.
OurMath - a class that contains the mathematical calculations in the Euclidean distance calculation program and direction vector calculation.
Animation - a class that objects of type AnimationData, Time and sprite and initializes animation.
   AnimationData – a class that holds a structure containing a vector of blocking rectangles, i.e. the organization for each animation.
Frog - a class that inherits from DynamicObject and initializes the game's frog for us. She holds about the player and updates them accordingly, and is responsible for her movement (using the mouse). In addition, she holds her bullets - the bullet in her mouth and the next bullet that will be in her mouth after we shoot the bullet in her mouth.
Ball - a class that inherits the DynamicObkect class. Creates a dynamic ball-type object, which holds its speed, color and direction as well as the next node in the path. It moves/stops itself according to the desired situation and also checks if there is a collision with a bullet fired from the frog.
Hole - a class that inherits from BaseObject, will start the end hole of the balls.
Road - class inherits from BaseObject, initializes a road type object.

data structures:

A pair vector that contains the indices of balls that start from a segment in the track.
A vector of coordinates of type Vector2f which contains the track nodes.
A vector of objects of type Road that contains the trajectory of the balls.
A vector of unique pointers to the Ball object containing the balls in the track.
A vector of pointers of type unique to the Ball object containing the balls that are fired.
A vector of unique pointers to the Ball object containing the balls in the track.
A struct that contains a vector of IntRect objects for the animations.
A vector of Button objects.
Texture vector in which we will store all the textures displayed during the game.
An array of type sf::SoundBuffer and an array of type sf::Sound in which we will store all the sounds played during the game.
Two vectors containing strings of the file names of the images and sounds that are loaded from the disk.
A vector of type AnimationData that contains the animations in the game.

Noteworthy algorithms:

• We calculate a direction vector for the benefit of moving the frog with the mouse, directing the balls in the track, activating the animations in the game, checking turns, etc.

Known bugs:

- Sometimes, when the cursor moves a lot near the center of the image of the frog, the balls it 'holds' (the next ball we shoot, and the ball after that) disappear.
- When we shoot a ball to the end of the track, that is to say at some turn, the space between the ball we inserted and the next ball is slightly larger than the desired space.

Remarks:
When we blew up balls of the same color and then got at the ends of the two new 'blocks' created balls of the same color as well, so the balls have to go back on the track. In light of problems and bugs in certain edge cases that we were not successful in solving and due to time pressure, we gave up on this case, and now the balls do not go back but stop.
  
Input file format:
The input file is of type txt and is called Path.txt
Adding a stage: If the user wants to add a new stage to the game, the user must open the "Path.txt" file and add a line break (\n), in a new line add the coordinates of the frog and the hole (with a space between each coordinate) and immediately after the coordinates of the nodes of the route. Finally, it is required to insert the coordinate of the hole in addition, in order to draw the path of the node before the hole to the hole. For example:
frog_x frog_y hole_x hole_y node1_x node1_y node2_x node2_y ………. hole_x hole_y
