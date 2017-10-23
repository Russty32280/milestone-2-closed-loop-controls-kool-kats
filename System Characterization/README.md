# Lab 6: Taking Control Over your Embedded Life
Now that your processor has taken its first glimpse at the world around, it is now time to teach it how interact with other things. Up to this point all you have taught your processor to do is count and turn something on or off. But in this world, only a small amount of things work like a switch. Most things require some form of resolution between them instead of gross movement. It is your task in your lab to teach your processor fine motor skills and precisely control something in the real world. 

## Goals for the lab:
By the end of this lab, you should:
* Interface your microcontrollers to multiple actuator types
* Design control circuitry to both drive the actuators as well as protect your microprocessor.
* Understand the bare fundamentals of open loop control and system modeling.

## Deliverables
By the end of the lab, you will need to provide at a minimum well documented main.c files for *EACH* of the development platforms used in the exercise. You will also need to provide a README at the top of each section in your repository. So come Milestone time, I should see 3 folders in your repository with the following titles:
* High Power Control
* Precision Control
* Open Loop Systems

each with the .c and README files for each board. The .c and README files should include the minimum amount of work as well as any extra work that you have done. Each processor should have its project saved in its own folder in each part of the assignment, however you only need one README per part of the assignment.

### README Files
Since most of these projects will have just a simple main.c file, you do not need to generate 20 README files. Instead, unless you go for a more advanced implementation of the exercises, you just need 1 README per exercise folder. "But how do I make a README with all of the processors included?" Well now we are getting somewhere. You should talk about the general form of your code as it should be very similar for each processor, but you should highlight what the differences are between each processor. For example, do the clocks need to be initialized differently? As another step forward, you could take that information and somehow make it where your code would work on any of the processors without the need to change it between projects.

### Header Files
You may find yourself by the end of this lab not having generated any header files, which in this case, ignore this section. If you have generated more than just a main.c you most likely have yourself a .h file or two. Remember from the first lab that any header files or libraries that you generate need to have their own README.md which tell the user what dependencies there are, how to install them into their projects, etc.

## Documentation
Since you will most likely be using pre-made code to make most of your code, so what I am going to require you to do is comment each line of code and what it is doing. If you are using code from TI themselves, it will most likely have some of the comments there. But I want you to use your own words when writing these comments. Dive into the datasheets, look into the MSP430.h file to see what registers are being manipulated and tell me why you need to configure them. 
