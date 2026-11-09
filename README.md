# 🚇 Metro & Bus Transit Network Manager

![C++17](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-3.10%2B-064F8C?logo=cmake&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Dijkstra%20Shortest%20Path-blue)
![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)

An interactive **C++17** urban public transit network management and route optimization system. It models bus, metro, and tramway lines, computes shortest passenger paths via **Dijkstra's Algorithm**, identifies multi-line transfer hubs, and analyzes network ridership statistics.

---

## ✨ Key Features

- **Multi-Modal Transit Graph:** Loads stops (`data/stops.txt`), inter-stop distances (`data/distances.txt`), transit lines (`data/lines.txt`), and passenger trip logs (`data/passengers.txt`).
- **Dijkstra Shortest Path Routing:** Computes minimum-distance routes between any two stops (supporting both stop IDs and stop names) and detects line transfers along the path.
- **Transfer Hub & Ridership Analytics:** Automatically identifies multi-line transfer stations (`Aktarma Noktaları`) and ranks the most heavily utilized transit lines.
- **Interactive Management CLI:** Allows adding/removing stops and transit lines dynamically at runtime via an interactive console menu.

---

## 🗂️ Project Structure

```text
Metro-Bus-Network/
├── data/
│   ├── distances.txt        # Inter-stop distance edges (meters)
│   ├── lines.txt            # Bus, Metro & Tram line stop sequences
│   ├── passengers.txt       # Passenger origin-destination trip records
│   └── stops.txt            # Stop IDs and station names (UTF-8)
├── include/
│   └── transport_system.h   # Graph structures & TransportSystem class API
├── src/
│   ├── main.cpp             # Interactive CLI menu & bootstrap
│   └── transport_system.cpp # Dijkstra router, hub analyzer & file parsers
├── CMakeLists.txt           # Cross-platform CMake build configuration
├── LICENSE                  # MIT License
└── README.md                # Project documentation
```

---

## 🚀 Build & Run Instructions

### Build with CMake
```bash
git clone https://github.com/Firatdondar/Metro-Bus-Network.git
cd Metro-Bus-Network
mkdir build && cd build
cmake ..
cmake --build .
./app
```

### Build Directly with g++
```bash
g++ -std=c++17 -Iinclude src/main.cpp src/transport_system.cpp -o app
./app
```

---

## 📄 License

Distributed under the **MIT License**. See [`LICENSE`](LICENSE) for details.
