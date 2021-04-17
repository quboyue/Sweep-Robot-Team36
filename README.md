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
        <ul>
      <li><a href="#algorithm">Algorithm</a</li>
      </ul>
      <li><a href="#hardware">Hardware</a</li>
      <li><a href="#test_map">Test_map</a</li>
       <li><a href="#roadmap">Roadmap</a</li>
      </ul>
    <li><a href="#getting-started">Getting Started</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#social-Media">Social-Media</a></li>
    <li><a href="#contact">Contact</a></li>
</ol>
     
***



## About the project

The project is based on the **Raspberry Pi 3**, using **3D printing model** and **LIDAR** scanning to realize the obstacle avoidance movement of the robot, and to remember the movement trajectory.


### Software

- [Opencv 4.5.1](https://opencv.org/opencv-4-5-1/)
- Python GPIO  
#### Algorithm 
 - **ICP**
 - **MCL**  
  This is an algorithm that calculates the position of a robot in a known map. The principle of this algorithm is to simulate a group of particles that have the same movement   as the robot. Each particle is given a certain probability by comparing the sensor data of robot with the simulated sensor data of the particles. After several iterations,   the particles will converge to a position, which is the exact position of the robot in the map.
 - **Floodfill**
 - **DFS**
 - **DWA**   
   This is an algorithm that plans out specific action strategies for the robot in a part of map after obtaining the information of destination. The principle is to sample      several groups of velocities and simulate the motion trajectory of the robot in a certain time at these velocities. Then an evaluation function is used to score these        trajectories to exclude collision trajectories and select the speed corresponding to the optimal trajectory to drive the robot.

***


### Hardware  

- [Raspberry Pi 3 Model B](https://www.raspberrypi.org/products/raspberry-pi-3-model-b/)
- **Sensor**  
     [Radar](https://www.slamtec.com/en/) (slamtec a1m8-r5)
- [electronic compass module](https://www.amazon.com/QMC5883-Electronic-Compass-Three-axis-Magnetic/dp/B07ZR79HGS)（QMC5883）
- [2*42 Stepper motor](https://www.amazon.com/3D-Printer-printer-4248-450-Stepper/dp/B07L1RMWZR/ref=sr_1_3?dchild=1&keywords=4248+Stepper+motor&qid=1618642588&sr=8-3)（424812）
- 2*Motor Driver(DM422)
- Mobile Power Supply
- Electric Level Transform Chip
- [3D drift](https://github.com/quboyue/Sweep-Robot-Team36/blob/main/Car_model.stl)  

<img width="550" height="400" src="https://github.com/GANTIAN-hub405/picutre/blob/main/3D.png"/></div>



-Schematic diagram of hardware connection

<img width="700" height="500" src="https://github.com/Nuko-XCB/material/blob/main/Hardware%20connection.png"/></div>




### Test_map:


<img width="550" height="400" src="https://github.com/GANTIAN-hub405/picutre/blob/main/map.png"/></div>

### Roadmap
See the [open issues](https://img.shields.io/github/issues/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge) for a list of proposed features (and known issues).


***
## Getting Started




## Contributing  

if you wish to contribute, please follow the following steps:  

1. Fork the project repository
2. Clone or download the project repository
3. Create a new branch
4. Make suitable changes and commit them
5. Push the changes
6. Submit a pull request  

If you are unsure about how to contribute, spot any problems or have suggestions on how we can improve, feel free to get in touch with us, our contact details can be found in the contact section.   

## License  
Project is distributed under MIT License  

## Social-Media
 Follow our social channels to keep up to date with our branding.
 - [Youtube](https://www.youtube.com/channel/UCdpiw0mUYiLdALUTy5392jA)
 - [Twitter](https://twitter.com/home)

## Contact
This project is being completed by a team of students at the University of Glasgow:  
 - Qu Boyue- [@Quboyue](https://github.com/quboyue) - <2560655Q@student.gla.ac.uk>
 - Gan Tian - [@Gantian](https://github.com/GANTIAN-hub405) - <2533183G@student.gla.ac.uk>
 - Bai Xiaochen - [@Baixiaochen](https://github.com/Nuko-XCB) - <2517191B@student.gla.ac.uk>
