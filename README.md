<h2 align="center">Sweep-Robot</h2>  



<div align=center><img width="550" height="400" src="https://github.com/GANTIAN-hub405/picutre/blob/main/sweep-robot.jpg"/></div>


<div align="center">
  
[![Contributors](https://img.shields.io/github/contributors/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/graphs/contributors)
[![Forks](https://img.shields.io/github/forks/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/network/members)
[![Stars](https://img.shields.io/github/stars/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/stargazers)
[![Issues](https://img.shields.io/github/issues/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/issues)
</div>

<!-- TABLE OF CONTENTS -->

# Table of contents

<ol>
    <li><a href="#about-the-project">About The Project</a></li>
      <ul>
      <li><a href="#software">Software</a</li>
      <li><a href="#hardware">Hardware</a</li>
      <li><a href="#test_map">Test_map</a</li>
       <li><a href="#roadmap">Roadmap</a</li>
      </ul>
    <li><a href="#getting-started">Getting Started</a></li>
      <ul>
      <li><a href="#algorithm">Algorithm</a</li>
      </ul>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#social Media and Outreach">Social Media and Outreach</a></li>
    <li><a href="#contact">Contact</a></li>
</ol>
     
***



## About the project

The project is based on the **Raspberry Pi 3**, using **3D printing model** and **LIDAR** scanning to realize the obstacle avoidance movement of the robot, and to remember the movement trajectory.


### Software

- [Opencv 4.5.1](https://opencv.org/opencv-4-5-1/)
- Python GPIO


### Hardware 
- **Raspberry Pi 3 Model B**
- **Sensor**  
     Radar (slamtec a1m8-r5)
- [3D drift](https://github.com/quboyue/Sweep-Robot-Team36/blob/main/Car_model.stl)  


<img width="550" height="400" src="https://github.com/GANTIAN-hub405/picutre/blob/main/3D.png"/></div>


### Test_map:


<img width="550" height="400" src="https://github.com/GANTIAN-hub405/picutre/blob/main/map.png"/></div>

### Roadmap
See the [open issues](https://img.shields.io/github/issues/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge) for a list of proposed features (and known issues).


***
## Getting Started



### Algorithm 
 - **ICP**
 - **MCL**  
  This is an algorithm that calculates the position of a robot in a known map. The principle of this algorithm is to simulate a group of particles that have the same movement   as the robot. Each particle is given a certain probability by comparing the sensor data of robot with the simulated sensor data of the particles. After several iterations,   the particles will converge to a position, which is the exact position of the robot in the map.
 - **Floodfill**
 - **DFS**
 - **DWA**   
   This is an algorithm that plans out specific action strategies for the robot in a part of map after obtaining the information of destination. The principle is to sample      several groups of velocities and simulate the motion trajectory of the robot in a certain time at these velocities. Then an evaluation function is used to score these        trajectories to exclude collision trajectories and select the speed corresponding to the optimal trajectory to drive the robot.

***
## Contributing
## License
## Social Media and Outreach
 Follow our social channels to keep up to date with our branding and public outreach.
 - [Youtube](https://www.youtube.com/channel/UCdpiw0mUYiLdALUTy5392jA)
 - [Twitter](https://twitter.com/home)

## Contact
This project is being completed by a team of students at the University of Glasgow:  
 - Qu Boyue- [@Quboyue](quboyue@gmail.com)
 - Gan Tian - [@Gantian](https://twitter.com/GanTian10640151)
 - Bai Xiaochen - [@Baixiaochen](nukobaixiaochen@gmail.com)
