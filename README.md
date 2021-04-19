<h2 align="center">Sweep-Robot</h2>  



<div align=center><img width="850" height="600" src="https://github.com/GANTIAN-hub405/picutre/blob/main/sweep-robot.jpg"/></div>

<h2 align="center">Glasgow Team36</h2>  
<div align="center">
  
[![Contributors](https://img.shields.io/github/contributors/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/graphs/contributors)
[![Forks](https://img.shields.io/github/forks/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/network/members)
[![Stars](https://img.shields.io/github/stars/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/stargazers)
[![Issues](https://img.shields.io/github/issues/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/issues)
[![LICENSE](https://img.shields.io/github/license/quboyue/Sweep-Robot-Team36.svg?style=for-the-badge)](https://github.com/quboyue/Sweep-Robot-Team36/blob/main/LICENSE)
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

  This project is an intelligent sweep robot developed based on **Raspberry Pi 3**. Its frame is built using **3D printing** and it can automatically avoid obstacles, clean the floor, and therefore reduce the user's housework.

  Users only need to place the robot in their room, where it automatically scans the surrounding environment of the room by the **LIDAR** and builds a grid map. After the map is built, the robot will make global path planning to determine the rough sweep route and start work. If the robot detects an obstacle that does not match the pre-stored map during the work, it will use the local path planning algorithm to choose the optimal route and avoid impacting the obstacle, so that your floor will be thoroughly cleaned.

Hands-free, let the robot clean your floor!



### Software

- [Opencv 4.5.1](https://opencv.org/opencv-4-5-1/)
- Python GPIO  
#### Algorithm 
 - **ICP**   
This is an algorithm used to build a map when the robot first arrives in an unknown environment.The principle is to analyze the point set scanned by the radar before and after each displacement, and then find the average displacement value of each point, which is regarded as the displacement of the robot. Through these data, the exact position of the robot relative to the origin can be calculated. The position of obstacles relative to the origin is calculated by combining the sensor data and the robot position, and as a consequence, the map is built.
 - **Map building**    
When the robot uses ICP algorithm to build the map, each scan will get the data of the surrounding environment, which can be divided into two types: blank space and obstacles. The principle of the map building is to darken the color of the obstacle position and lighten the color of the blank position in each scan (blue in this project). A complete map can be created by superimposing all the results of scans.   

<div align=center><img width="500" height="375" src="https://github.com/Nuko-XCB/material/blob/main/Grid%20map.png"/></div>  

 - **MCL**    
  This is an algorithm that calculates the position of a robot in a known map. The principle of this algorithm is to simulate a group of particles that have the same movement   as the robot. Each particle is given a certain probability by comparing the sensor data of robot with the simulated sensor data of the particles. After several iterations,   the particles will converge to a position, which is the exact position of the robot in the map.
  
<div align=center><img width="300" height="150" src="https://github.com/Nuko-XCB/material/blob/main/MCL.png"/></div>  


 - **Floodfill**    
 This algorithm is used to detect the region that the robot can reach when the robot is running. The principle is to look for all nodes that are connected to the start node and then form a connected area where the robot can travel.
 - **DFS**    
This is an algorithm for planning the path of a robot. Depth first search path planning starts from the starting point, chooses one direction and keeps going in this direction until it encounters obstacles, and then tries another direction until it finally comes to the endpoint. The advantage of depth first search is that they can explore every branch as much as possible. This algorithm is more efficient because it does not repeat the route which is already reached before. Compared with the breadth-first algorithms, which traverse every layer of data, this method is more efficient.
 - **DWA**   
   This is an algorithm that plans out specific action strategies for the robot in a part of map after obtaining the information of destination. The principle is to sample      several groups of velocities and simulate the motion trajectory of the robot in a certain time at these velocities. Then an evaluation function is used to score these        trajectories to exclude collision trajectories and select the speed corresponding to the optimal trajectory to drive the robot.
   
<div align=center><img width="375" height="215" src="https://github.com/Nuko-XCB/material/blob/main/DWA.png"/></div>  

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

<img width="850" height="600" src="https://github.com/GANTIAN-hub405/picutre/blob/main/3D.png"/></div>



-**Schematic diagram of hardware connection**

<img width="700" height="500" src="https://github.com/Nuko-XCB/material/blob/main/Hardware%20connection-final2.png"/></div>




*** 
### Test_map:


<img width="850" height="600" src="https://github.com/GANTIAN-hub405/picutre/blob/main/map1.jpg"/></div>  

<img width="850" height="600" src="https://github.com/GANTIAN-hub405/picutre/blob/main/roadmap.png"/></div>  


***
### Roadmap  

- Completed the ICP algorithm and built the room map.
- Completed MCL algorithm to realize robot positioning.
- Complete the global path planning algorithm and help the robot to determine the route.
- Completed the DWA algorithm and helped the robot avoid obstacles.
- KdTree is used to store point cloud and accelerate ICP algorithm.
- Use floodfill algorithm to optimize feasible positions in global path planning.  

Future plans:  
- Use a wheel odometer to assist positioning.
- Optimize MCL algorithm.
- Optimize MCL into AMCL algorithm.





***
## Getting Started  

  **Prerequists:**  
  1. install opencv-4.5.1  
     ```  
     //Install the necessary build tools and common toolkits  
     
      sudo apt-get install build-essential cmake git pkg-config  
      sudo apt-get install libjpeg8-dev  
      sudo apt-get install libtiff5-dev  
      sudo apt-get install libjasper-dev  
      sudo apt-get install libpng12-dev  
      sudo apt-get install libavcodec-dev libavformat-dev libswscale-dev libv4l-dev  
      sudo apt-get install libgtk2.0-dev  
      sudo apt-get install libatlas-base-dev gfortran  
      
      //Download the opencv  
      
      wget -O opencv-4.5.1.zip https://github.com/Itseez/opencv/archive/4.5.1.zip  
      unzip opencv-4.5.1.zip  
      wget -O opencv_contrib-4.5.1.zip https://github.com/Itseez/opencv_contrib/archive/4.5.1.zip  
      unzip opencv_contrib-4.5.1.zip  
      
      //  Will download the source decompression, and enter the source directory in the terminal
      cd ~/opencv-4.5.1  
      
      mkdir release  
      cd release  
      sudo cmake -D CMAKE_BUILD_TYPE=RELEASE \
      -D CMAKE_INSTALL_PREFIX=/usr/local \
      -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-3.4.1/modules \
      -D INSTALL_PYTHON_EXAMPLES=ON \
      -D BUILD_EXAMPLES=ON ..

     ```  
     
  2. install wiringPi  
   ```  
   Note: the wiringpi used in the project is 64 bit version.   
   If you use the official image of raspberry pie, wiringpi has been pre installed and can be used       directly.
     git clone https://github.com/WiringPi/WiringPi.git
     cd ./WiringPi
     ./build
   ```
  
  **(optional): Python**  
```  
     sudo apt-get install python3-rpi.gpio  
```  

  **Installation**  
```  
    git clone https://github.com/quboyue/Sweep-Robot-Team36.git
    cd Sweep-Robot-Team36/Sweep_Robot/build
    sudo camke ..
    make  
```  

  **Usage**  
```  
   ./main 
```

***

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
Project is distributed under [MIT License](https://github.com/quboyue/Sweep-Robot-Team36/blob/main/LICENSE)

## Social-Media
 Follow our social channels to keep up to date with our branding.
 - [Youtube](https://www.youtube.com/watch?v=rYhGWg9WMtA)  
 - [Bilibili](https://www.bilibili.com/video/BV1k64y1m7D2/)


## Contact
This project is being completed by a team of students at the University of Glasgow:  
 - Qu Boyue- [@Quboyue](https://github.com/quboyue) - <2560655Q@student.gla.ac.uk>
 - Gan Tian - [@Gantian](https://github.com/GANTIAN-hub405) - <2533183G@student.gla.ac.uk>
 - Bai Xiaochen - [@Baixiaochen](https://github.com/Nuko-XCB) - <2517191B@student.gla.ac.uk>  
 
 **Project Link**:<https://github.com/quboyue/Sweep-Robot-Team36>
