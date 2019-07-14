# forklift-sim
Simulation of a line-following forklift robot with a PID controller

![Screenshot of the simulation](https://i.imgur.com/nUQmeUo.jpg)

## What's the point of this?
This program was developed to check whether some assumptions made during the construction of a forklift robot were correct.

They weren't.

### What were the assumptions?
1. Robot would be steered with a single central rear wheel
2. That wheel would revolve left and right with a particular maximum angular velocity which resulted from the combination of the servo and gearing used
3. Light reflection sersors would be placed ahead of the front axle to detect the changing lateral position of the followed line

### How were they wrong?
Inspired by real life forklifts, the rear wheel steering seemed like the right idea. Unfortunately, it wasn't and made every other assumption wrong as well. The effectively backwards layout of the vehicle made it very difficult to achive good line following performance. Correct steering action would pivot the sensors into a deeper error and result in further corrections, extreme fishtailing and very frequent complete failures at following the line.

The fairly low steering speed resulting from using a LEGO motor and some questionable gearing introduced a significant lag between the steering angle requested by the controller and the actual angle at which the wheel was at. Because of that the robot had to drive forward really slowly and even then the behaviour was very unpredictable.

The wrongness of this approach was further emphasized by a really quick test run with the robot driving backwards so that the steered wheel was in front. The only change was moving the sensors so that they would be in the middle of the forklift lengthwise. The result was shocking, the quickly set-up reverse layout outperformed a finely tuned normal layout in every metric. It followed the line more closely, didn't fishtail and could drive up to five times faster with the same slowly turning steering.

The assumed layout of the robot was a disaster.

### How did that help?
Based on the results, the team decided to instead go with a classic tank-steer approach. Instead of driving both front wheels with a single motor through a differential, we would repurpouse the motor that would steer the rear wheel to instead drive one of the front wheels, while the other motor would drive the other wheel. The rear wheel was replaced with a castor (another source of problems, but that's a story for another day).

That layout was much simpler to build and program and we should have gone with it from the start, but for some reason we had that other idea.

### Why was this even made in the first place?
If you're wondering if it would be easier to just build the robot (it's made of LEGO that's what they're for) and check the assumptions that way instead of programming a weird simulation, I'd say that it wouldn't.

The iteration rate was much higher that would be possible with the physical equipment. The controller was very slow to flash, LEGO pieces were hard to find and building was difficult due to limited access to the workshop and the team's limited experience with LEGO Technic.

## What's going on on the screen
- The grid in the background marks 10 centimeter intervals.
- The 3 circles that change from black to white represent the sensors
- The magenta circle shows the path that the rear wheel will take based on the angle and wheelbase
- The thick black line is the path the robot should follow

## Tuning stuff
Unfortunately you have to look all around the source code to change parameters. It could use some functionality to load them from a file, so you don't have to recompile all the time but it was a single use thing so I figured it wouldn't make sense to write that functionality.

Creating new paths is also not as straight forward as it could be, the syntax is obtuse and the validation function is a joke. A better way would be to just use an image anyway.

## Building the software
- On Linux (and probably OSX I dunno), install the `libsfml-dev` library and run the `makefile`.
- On Windows, use the WSL I guess

## Additional stuff
Check out https://github.com/mussorgsky/fork-kaiser to see how it turned out in the end.
