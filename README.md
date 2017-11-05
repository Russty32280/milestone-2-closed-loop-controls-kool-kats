# Milestone 2: Closed Loop Systems
In Lab 6 you needed to characterize and control an Open Loop system. Once you actually close the loop, the system dynamics as a whole might change, meaning you might need to take a different strategy for your controller.

## Closing the loop
For starters, you need to create a closed loop system. To do this, take the temperature you are reading from the 5V regulator find the difference between it and the desired value. This desired value can either be hard-coded into your program or received over UART at the beginning. For the final testing of your system, you need to be able to set the set point without the need to re-program your microcontroller (cough, UART or Pot, cough cough). This value should then be used to determine the PWM Duty Cycle of your fan. You will want to capture a plot of the performance of your system over a period of about 10-30 seconds so you can track how your system is doing. Try this with a few different set points and note how the system is reacting.

## Taking your measurement into account
Now that you get to see what happens when you feed just the difference or error signal into your control system, what way can we make this better. This is really the focus of this Milestone is to explore ways to close this loop. You might want to try taking this difference and subtracting this from the set point. Or maybe you try to re-characterize your system and utilize Look-Up tables or more complex models to track the behavior of your system. You can implement a control algorithm to attempt to modulate the fan based on certain trends in the temperature. This is totally up to you. What matters the most is can you show to a user what the temperature of the chip currently is, and can your system hold the temperature over a period of time.

## Displaying Temperature
Other than on the oscilloscope, you should be able to track the temperature of your system over a period of time. This can be done using MATLAB, the serial plotter in Code Composer, or another means that you can find. This is so that we can track your system over a period of about 2-5 minutes. It might be useful to also use an LCD display or some other method to see right away what your system is doing. You will need in the App Note for this Milestone this data so that you can describe the performance of your system.

## System Performance
For testing, you will be asked to do a few scenarios where your system will try to stabilize around a specific temperature within a period of time. You will be asked to start with your system at room temperature and bring the regulator up to a specific temperature. While your system is approaching steady state, the main thing we are going to care about is the speed at which you approach it as well as the ability for your system to stay stable. After this first test, you will then be asked to bring your system back to a temperature warmer than room temperature and be analyzed again. Then your system will need to bring the chip up to a mildly hot temperature, stabilize, then we will introduce "disturbances" in your system to see how it reacts. These will include increasing/decreasing the voltage across the regulator, a heavier load for your regulator to drive, and adding an additional fan.

* Room Temp -> Medium/Hot
* Medium/Hot -> Slightly Warm
* Slightly Warm -> Medium/Hot
* Add Disturbances

In summary, the testing procedure will take about 20-30 minutes in total, so you MUST come to the lab period ready to demonstrate in a group setting. If you do not come ready, your team will be penalized and we will try to accommodate an additional testing time. If you can not show your system by that time, the portion of your grade depending on the system performance will become a zero. This doesn't mean you will get a zero for the milestone, but your grade will suffer. 

## Grading
Your grade will be dependent not only on your code, commenting, and report, but also on the performance of the system during your testing. The Milestone Grade is broken down as:

* 30% App Note
* 30% Code and commenting
* 10% Room Temp to Hot
* 10% Hot to Warm
* 10% Warm to Hot
* 10% Disturbance Resilience

For the actual tests, your system will be graded on two main factors, stability and steady state performance. Each of the 10% portions will be broken up into 7% and 3% for the Steady State and Stability Respectively. The brackets for these grades are:

### Steady State (7%)
* 7 points: Steady state performance is within 3C of the specified temperature
* 6 points: Steady State performance is within 5C of the specified temperature
* 5 points: Steady State performance is within 7C of the specified temperature
* 4 points: Steady State performance is within 9C of the specified temperature
* 3 points: Steady State performance is within 11C of the specified temperature 
* 2 points: Steady State performance is above/below 11C of the specified temperature
* 0 points: No steady state temperature was able to be recorded.

### Stability (3%)
* 3 points: The system shows little to no oscillations around 3C of the specified temperature after the specified time for testing.
* 2 points: The system shows steady but small oscillations around 5C of the operating point.
* 1 point: The system has heavy oscillation but is still stable. This may audible in the fan changing speed rapidly.
* 0 points: There is no stability or the system temperature can not be measured.


## Deliverables
Along with your performance marked during testing, you will need to submit your code along with an App Note in your team repository. These team repositories will be kept private this time around in an attempt to allow each team to get creative. Feel free to utilize code from other labs you have done (which is actually the point of making you all do these labs in the first place) and look around and research what other people do for these types of systems. 

In your app note and your repository, I expect you to have a section dedicated to the hardware used in the system and how it is connected. You will need to specify what sensors you are using, why use those sensors, and how you condition the signal before taking it into your microcontroller. As for the software side of things, your big focus in both App Note and code comments should be on the method in how to get information from your system, how to place the set point, and most importantly how your control algorithm works. This can not be stressed enough. 

You will need to include a block diagram to show your system components and define the signals going between them and their relationship to each other. This really needs to stay "high level" and should look at the system components in terms of transfer functions. In your App Note, you need to include small portions of code exemplifying your algorithm and approach. As with Milestone 1, you need to really give us engineering analysis and decision making as to why you went with your algorithm and talk about what lead you to pick your specific implementation. Do we expect you to be Control Theory experts? No, but we do expect you to be able to take steps towards analyzing what is not desirable in your system and how to remedy it. 