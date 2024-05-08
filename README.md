# Soft World

![physical simulation](https://img.shields.io/badge/physical%20simulation-F3A505)
![LGPL-3.0 license](https://img.shields.io/badge/license-LGPL--3.0-green)

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

### 1.Soft shell with ideal gas

The main idea is ideal gas law: $PV = nRT$

It provides normal points acceleration: $a_n = GC * \frac{l}{S^3}$

$S$ is (visible)area of the shell contents

$d$ is (invisible)depth of the contents

$m/d$ is mass of the body divided by $d$

$l$ is length of shell's part

$GC$ is gas constant

Shell also have rigidity to keep the shape of body:

It provides tangential points acceleration: $a_t = SR * (\frac{l}{l_0} - 1)$

$SR$ is shell rigidity

$l_0$ is initial length of shell's part

The density of the gas depends on $S$:

$\rho = \frac{m/d}{S}$

The shell also bounces off the walls, maintaining some part of the speed:

$v_{new} = SB * v_{old}$

$SB$ is shell bounce

### 2.Spring-mass body model

The body is simulated as rectangualr grid of massive points connected by horizontal, vertical and diagonal springs. Additionaly, each spring adds some 'friction' force proportional to relative speed in its direction. Collisions are simulated by adding impulse to points colliding with things proportionally to how deep the collision is. To conserve impulse, total impulse of collision between two bodies is proportional to their area of intersection.

x and y point grid scales can be adjusted separately.
Stiffness controls the stiffness of each spring.
Resistance controls the amount of friction force applied by each spring for relative speed of 1.
Density controls the mass of each point
