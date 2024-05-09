# Project Overview
The Philosophers project tasks students with simulating the "Dining Philosophers" problem, where a group of philosophers must share a limited number of forks while adhering to certain rules. The primary challenge is to ensure that the philosophers do not encounter deadlock or starvation while dining, demonstrating proper use of threading, mutexes, and synchronization techniques.

# Key Features
Multithreading: Implement a multithreaded simulation to model the behavior of concurrent philosophers.
Synchronization: Use mutexes or other synchronization mechanisms to manage resource sharing and avoid conflicts.
Deadlock Avoidance: Design algorithms to ensure that philosophers do not get stuck in a deadlock.
Starvation Prevention: Ensure that each philosopher has a fair chance to eat without waiting indefinitely.
Dynamic Parameters: Allow configuration of key parameters such as the number of philosophers, time to eat, time to sleep, and more.
# Getting Started
To clone this repository and build the Philosophers project:

    git clone https://github.com/Geesama02/Philosophers.git
    cd Philosophers/philo
    make
    ./philo 2 800 200 200

After building, you can run the simulation with customizable parameters to observe the behavior of the philosophers and ensure proper synchronization.

# Learning Objectives
- Concurrency and Multithreading: Understand the principles of concurrent programming and how to implement multithreaded applications.
- Resource Management: Learn techniques to manage shared resources without causing deadlocks or other synchronization issues.
- Problem Solving: Develop algorithms that solve the Dining Philosophers problem with optimal efficiency and fairness.
- Robustness and Testing: Create a robust system that can handle various edge cases and parameter configurations.
