# Project "drift"

The goal of this project is to create a fun 2D racing game with emphasis on realistic drifting.

# Development process

Initially I had two ideas.
Simulate each wheel as a separate object with point mass and then connect it with some sort of spring and dumper structure or create a singular car with both mass and moment of inertia.
I chose the second approach as it seemed simpler. Development of a different game engine may be an interesting exercise in the future.

The next step was to calculate forces acting on the wheels. To that end I created a small and simple library to handle 2D vectors.
I did not test the library before writing the *car_physics()* function which proved to be a mistake and caused a quite long debugging session.

The logic used was quite simple.
When a wheel has a speed vector that is not inline with the wheels plane of rotation, the sideways speed should be countered with force, so that the wheels don't "slide".
The length of that force vector I just left as is, directly proportional, to the speed vector.
The rest of the parameters of the car where adjusted to avoid jitter.
This approach surprisingly gave very good results as the car was turning with the same radius, for a given *front_wheel_angle* regardless of speed. 
Next step was adding a limiter on how big a force can be to introduce sliding on a surface, then adding slightly different limits for static vs dynamic friction.
At this stage the game behaved approximately how I would like and I even managed to make an automatic driver that would try to maintain a constant angular velocity.

However the more I thought about the mechanics of this movement the less it made sense to me.
In a turn the inward force should be $F=mrv^2$ so the force should scale with the square of the speed not with speed directly.
How was the car staying in proper turn at any speed?
Another problem was that if the car has different ratios of mass to moment of inertia there should be different forces acting upon the wheels when the car is rotating vs sliding to the side, for the same speed of the wheels.

I have spent way too much time trying to solve this problem in my head and would probably be better of just focusing in a single night and using a piece of paper.
One Idea I had was that when there is side force applied to the wheel the wheel slides at a small angle even though it doesn't lose the static friction.
So I tried dividing the sideways speed by the forward/backward speed to get the proper force.
I also tried to simply scale the force with the square of the speed.
Both those approaches failed and left me with weirdly performing simulation.

Finally I thought of imagining the wheels are sort of mounted on a spring. The more displaced the wheels are the bigger the force should be.
We can think of the sideways part of a speed vector to be displacement and it could be kept track of by simply saving the displacement from the previous loop pass.
This seems to make the previous problems solved and also makes sense in my head.

# To do

1. possibly changing the language to C++
1. surface dependent max static friction
1. joystick input
1. waypoints, way of setting them up.
1. obstacles and collision detection
1. proper map, and car sprites
1. 2 player mode
