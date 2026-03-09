#include "transport_system.h"
#include <iostream>

int main() {
    TransportSystem system;
    
    // Veri yolları yeni İngilizce isimlere ve data/ klasörüne göre güncellendi
    std::string stopsFile = "data/stops.txt";
    std::string distancesFile = "data/distances.txt";
    std::string linesFile = "data/lines.txt";

    std::cout << "Loading system data..." << std::endl;
    
    system.createGraph(stopsFile, distancesFile, linesFile);
    system.showMenu();
    
    return 0;
}