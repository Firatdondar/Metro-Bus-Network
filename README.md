# Transportation Management System

A C++17 public transportation routing and network management system that models bus stops, transit lines, and inter-stop distances, calculating optimal passenger routes using **Dijkstra's Shortest Path Algorithm**.

## Features
- **Graph Network Modeling:** Loads stop definitions, distance matrices, and transit line routes from `data/`.
- **Dijkstra Route Planning:** Finds shortest paths and transfer points for passenger journeys (`data/passengers.txt`).
- **Line & Stop Analytics:** Identifies transfer hubs and tracks most frequently used transit lines.

## Build & Run

### Using CMake
```bash
mkdir build
cd build
cmake ..
cmake --build .
./app
```

### Using g++ Directly
```bash
g++ -std=c++17 -Iinclude src/main.cpp src/transport_system.cpp -o app
./app
```
