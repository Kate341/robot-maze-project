<h1 div align = "center", style = "color:red"> ESP32 Maze Solving Robot </h1>

Autonomous maze-solving robot based on the **Alashed Robot Phobo ESP32 4WD Smart Car Kit V3.0**.

The robot uses:
- ultrasonic obstacle detection,
- servo-based environmental scanning,
- finite state machine (FSM) navigation,
- autonomous movement inside a maze.

---
<h3 div align = "center"> Project Overview </h3>

This project implements an autonomous robotic platform capable of navigating through a maze environment using real-time sensor data and embedded control algorithms.

The system was developed in:
- C++
- Arduino IDE
- ESP32 platform

The robot scans the environment using an ultrasonic sensor mounted on a servo motor and makes navigation decisions dynamically.

---
<h3 div align = "center"> Hardware Components </h3>

Main hardware used in the project:

- ESP32 Development Board
- ESP32 Expansion Board / Sensor Shield
- L298N Motor Driver
- 4 DC Motors
- HC-SR04 Ultrasonic Sensor
- Servo Motor
- 2×18650 Battery Pack
- 4WD Robot Chassis

Unlike the standard Arduino Uno version of the Robot Phobo kit, this implementation uses an ESP32-based platform.

---
<h3 div align = "center"> Software Environment </h3>

- C++
- Arduino IDE
- ESP32 Board Support Package
- ESP32Servo Library

---
<h3 div align = "center"> Navigation Algorithm </h3>

The robot follows the navigation priority:

1. Turn left if possible
2. Move forward if the path is clear
3. Turn right if necessary
4. Perform U-turn in dead ends

This allows systematic maze exploration.

---
<h3 div align = "center"> Finite State Machine Architecture </h3>

The program is organized using a finite state machine (FSM).

Main states:
- MOVE_FORWARD
- SCAN_AND_DECIDE
- TURN_LEFT_STATE
- TURN_RIGHT_STATE
- UTURN_STATE

This architecture improves:
- modularity,
- readability,
- debugging,
- control flow organization.

---
<h3 div align = "center"> Ultrasonic Scanning System </h3>

The ultrasonic sensor is mounted on a servo motor for directional scanning.

The robot scans:
- left,
- front,
- right.

To improve measurement stability, multiple ultrasonic readings are averaged to reduce sensor noise.

---
<h3 div align = "center"> Motor Control </h3>

The robot uses differential drive control for:
- forward movement,
- left rotation,
- right rotation,
- stop,
- U-turn operations.

Movement is controlled through GPIO pins connected to the motor driver.

---
<h3 div align = "center"> Educational Value </h3>

This project demonstrates practical experience in:
- embedded C++ programming,
- ESP32 systems,
- robotics,
- autonomous navigation,
- sensor integration,
- motor control,
- real-time decision making.

---
<h3 div align = "center"> Project Structure </h3>

```text
src/        → Arduino source code
docs/       → Documentation
images/     → Robot photos and diagrams
```

---
<h3 div align = "center"> Future Improvements </h3>

Possible future extensions:
- PID motor speed control
- BFS maze-solving algorithm
- shortest-path optimization
- Bluetooth telemetry
- mobile application integration
- maze mapping and visualization on a laptop using Raspberry Pi

---
<h3 align = "center"> Conclusion </h3>

This project demonstrates the implementation of an autonomous embedded robotic system using ESP32 technology, combining sensor processing, motor control, and autonomous navigation algorithms in a real-world robotics application.
