## General information

It's simple simulation of soft bodies physics with **Qt** graphical interface.

You can add bodies from **Palette**. 

You can change settings of instantiated bodies or bodies in Palette or scene in **Inspector**.

Also you can select and move bodies by **mouse**.

## Shortcuts

There are several **shortcuts** to speed up your work with the simulation.

| Keys  | Actions |
|-------|---------|
I | hide/show inspector window
P | hide/show palette window
Ctrl + S | save scene
Ctrl + O | load scene
Alt + S | save palette
Alt + O | load palette
Q | cancel insertion
Del | delete selected body
Enter | run/stop simulation

## Restrictions

This simulation runs on **CPU only**.

If there are a lot of bodies on scene, it is possible,
that physical thread will cost so much CPU time. 
In this case you still can change simulation,
but your **changes may be canceled** and **frames may be skipped**.

Threfore in scene settings in **Inspector** you can change time between physics updates.
Or also you can **stop simulation**, that will interrupt physics calculations.

## Implemented algorithms

### 1.Soft ball with ideal gas

The main idea is ideal gas law: $PV = nRT$

It provides normal points acceleration: $a_n = \frac{GC}{m/d} * \frac{l}{S}$

$S$ is (visible)area of the ball 
$d$ is (invisible)depth of the ball
$m/d$ is mass of the ball devided by $d$
$l$ is length of shell's part
$GC$ is gas constant

Shell also have rigidity to keep the shape of body:

It provides tangential points acceleration: $a_t = SR * (\frac{l}{l_0} - 1)$

$SR$ is shell rigidity

The density of the ball depends on $S$:

$\rho = \frac{m/c}{d}$

The ball also bounces off the walls, maintaining some part of the speed:

$v_{new} = SB * v_{old}$

$SB$ is shell bounce

### 2........