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
Del | delete body/item/whatever when it selected
Enter | run/stop simulation

## Restrictions

This simulation runs on **CPU only**.

If there are a lot of bodies on scene, it is possible,
that physical thread will cost so much CPU time. 
In this case you still can change simulation,
but your **changes may be canceled**.

Threfore in scene settings in **Inspector** you can change time between physics updates.
Or also you can **stop simulation**, that will interrupt physics calculations.