# Gladiator Unreal Engine

## Presentation
-----------
![png](./Images/GameScreen_1.png)

### Introduction
-----------
 `Gladiator` is our second game project made on Unreal Engine 4. <br>
 The objective of this project is to realize a brawler game by using C++ scripts in Unreal Engine <br>
 This project has been made by **RABINNE Lenny** and **ERBISTI Matéo**. <br>
 It started on `October 24th, 2022` and finished on `November 21st, 2022`. <br>

### Description
-----------
In this brawler, your objective is to kill 5 enemies in the arena by using your hammer and your shield to protect yourself. <br>
You can also use an ultimate ability that will deal damage to every enemy. <br>


#### Controls :
-----------
You can play with a keyboard and mouse or with a controller.

Controls Keyboard and Mouse:<br>
`Move` : WASD <br>
`Turn the camera` : Mouse <br>
`Attack` : Left Mouse Button <br>
`Shield` : Right Mouse Button <br>
`Ultimate` : R <br>
`Lock/Unlock an Enemy` : Shift <br>
`Change Lock` : Mouse Wheel <br>
`Restart` : Enter <br>

Controls Controller:<br>
`Move` : Left Stick <br>
`Turn the camera` : Right Stick <br>
`Attack` : Right Trigger <br>
`Shield` : Left Trigger  <br>
`Ultimate` : Face Button Bottom <br>
`Lock/Unlock an Enemy` : Right Stick Button <br>
`Change Lock` : Right Shoulder <br>
`Restart` : Start <br>

#### Player: 
-----------

You will start the game with 5 lives. <br>
You can lock an enemy which makes it easier to go after him and attack him. <br>
Each time an enemy hit you or you deal damage to them, you'll earn an ultimate charge. With 6 ultimate charges, you can press the Ultimate button to make a special attack. <br>
The Ultimate Charge is represented with a Widget at the bottom right corner of the screen. <br>

#### Enemies : 
-----------
The enemies will stay within a good range of the player so that they can stay safe. <br>
They will also move to another location if another enemy is located between them and the player. <br>
Each second, the nearest Enemy to the player will rush you and attack. <br>
You can protect yourself by facing the enemy and holding your shield. <br>
Each Enemy has 3 lives, you can see their life bar by locking them. <br>

We used a Behavior Tree, blackboard and created some C++ Tasks to make our artificial intelligence. <br>
We found the Behavior Tree really useful and quite easy to use to create AI. <br.>

#### Difficulties: 
-----------

At the beginning of the project, it was kind of hard to approach C++ with Unreal Engine. <br>
There are a lot of things to know that are specific to Unreal such as UFUNCTION, UPROPERTY, all the specifiers, but also the architecture of Unreal <br>

We also struggled a bit to begin our artificial intelligence but we found good documentation that helped us understand how tasks, behavior trees, and blackboards work together. <br>


