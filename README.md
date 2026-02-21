## Game AI Behavior Simulation
A project I created for a Game AI class in 2024 to learn about AI behaviors and movement. Made with [openFrameworks](https://openframeworks.cc/). The project had two assignments, each with their own requirements and complexity:
- Assignment 1 focused primarily on dynamic behaviors such as steer, arrive, flock, and wander.
- Assignment 2 built upon the concepts in Assignment 1 by incorporating pathfinding algorithms such as A* and Djikstra's.

Each assignment includes a process letter that described the development process, highlighting roadblocks, successes, and areas for future improvement.

# Download

Check out the [releases](https://github.com/Fredrick117/AI-Movement/releases/tag/v1.0) page!

## Overview
This simulation can illustrate each of the following movement behaviors:
- Seek (a boid moves towards the target location, but does not stop when it reaches the location)
- Arrive (a boid moves towards the target location, slowing its velocity as it approaches the destination and eventually stopping)
- Wander (a boid moves around the world in a random fashion)
- Flock (a group of boids move together, like a flock of birds)

![boids1](https://github.com/user-attachments/assets/d2ee6309-048f-4520-b2a6-22638a473e11)

*The flock leader is represented in red, while the followers are represented in white. The flock leader is set to "wander", while the followers are actively "seeking" the leader*
