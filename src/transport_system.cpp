#include "../include/transport_system.h"

std::string TransportSystem::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void TransportSystem::createGraph(const std::string& stopListFile, const std::string& stopDistancesFile, const std::string& linesFile) {
    // 1. Load Stops
    std::ifstream stopsFs(stopListFile);
    if (!stopsFs.is_open()) {
        std::cerr << "Error: Could not open stops file: " << stopListFile << std::endl;
        return;
    }

    std::string lineContent;
    std::getline(stopsFs, lineContent); // Skip header
    while (std::getline(stopsFs, lineContent)) {
        lineContent = trim(lineContent);
        if (lineContent.empty()) continue;

        std::stringstream ss(lineContent);
        std::string idStr, name;
        if (std::getline(ss, idStr, ',') && std::getline(ss, name)) {
            try {
                int id = std::stoi(trim(idStr));
                name = trim(name);
                stops[id] = Stop(id, name);
                stopNameToId[name] = id;
            } catch (...) {}
        }
    }
    stopsFs.close();
    std::cout << stops.size() << " stops loaded." << std::endl;

    // 2. Load Distances (Matrix Format)
    std::ifstream distancesFs(stopDistancesFile);
    if (!distancesFs.is_open()) {
        std::cerr << "Error: Could not open distances file: " << stopDistancesFile << std::endl;
        return;
    }

    std::vector<int> sortedStopIds;
    for (const auto& pair : stops) sortedStopIds.push_back(pair.first);
    std::sort(sortedStopIds.begin(), sortedStopIds.end());

    size_t rowIndex = 0;
    while (std::getline(distancesFs, lineContent) && rowIndex < sortedStopIds.size()) {
        lineContent = trim(lineContent);
        if (lineContent.empty()) continue;

        std::stringstream ss(lineContent);
        std::string token;
        size_t colIndex = 0;
        int currentRowStopId = sortedStopIds[rowIndex];

        while (std::getline(ss, token, ',') && colIndex < sortedStopIds.size()) {
            token = trim(token);
            int currentColStopId = sortedStopIds[colIndex];
            if (token != "-" && !token.empty()) {
                try {
                    int distance = std::stoi(token);
                    if (distance > 0) {
                        distanceGraph[currentRowStopId][currentColStopId] = distance;
                        distanceGraph[currentColStopId][currentRowStopId] = distance;
                    }
                } catch (...) {}
            }
            colIndex++;
        }
        rowIndex++;
    }
    distancesFs.close();
    std::cout << "Distance matrix loaded." << std::endl;

    // 3. Load Lines (supports both Stop Names and Stop IDs)
    std::ifstream linesFs(linesFile);
    if (!linesFs.is_open()) {
        std::cerr << "Error: Could not open lines file: " << linesFile << std::endl;
        return;
    }

    std::getline(linesFs, lineContent); // Skip header
    while (std::getline(linesFs, lineContent)) {
        lineContent = trim(lineContent);
        if (lineContent.empty()) continue;

        std::stringstream ss(lineContent);
        std::string lineNoStr, lineName;
        if (!std::getline(ss, lineNoStr, ',') || !std::getline(ss, lineName, ',')) continue;

        int lineNo = 0;
        try {
            lineNo = std::stoi(trim(lineNoStr));
        } catch (...) {
            continue;
        }
        lineName = trim(lineName);

        Line currentLine(lineNo, lineName);
        std::string token;
        while (std::getline(ss, token, ',')) {
            token = trim(token);
            if (token.empty()) continue;

            int stopId = -1;
            if (stopNameToId.count(token)) {
                stopId = stopNameToId[token];
            } else {
                try {
                    stopId = std::stoi(token);
                } catch (...) {
                    stopId = -1;
                }
            }

            if (stopId != -1 && stops.count(stopId)) {
                currentLine.stops.push_back(stopId);
                stops[stopId].passingLines.insert(lineNo);
            }
        }
        lines[lineNo] = currentLine;
    }
    linesFs.close();
    std::cout << lines.size() << " lines loaded." << std::endl;
}

void TransportSystem::searchStop(int stopId) {
    auto it = stops.find(stopId);
    if (it != stops.end()) {
        std::cout << "Stop Found: ID " << stopId << ", Name: " << it->second.name << std::endl;
        if (!it->second.passingLines.empty()) {
            std::cout << "Lines passing through: ";
            for (int lineNo : it->second.passingLines) {
                std::cout << lineNo << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "No lines pass through this stop." << std::endl;
        }
    } else {
        std::cout << "Stop with ID " << stopId << " not found." << std::endl;
    }
}

std::pair<int, std::vector<int>> TransportSystem::calculateShortestPath(int startStopId, int targetStopId) {
    if (stops.count(startStopId) == 0 || stops.count(targetStopId) == 0) {
        return {-1, {}};
    }

    std::map<int, int> distances;
    std::map<int, int> previousStops;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    for (const auto& pair : stops) {
        distances[pair.first] = std::numeric_limits<int>::max();
    }

    distances[startStopId] = 0;
    pq.push({0, startStopId});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentStop = pq.top().second;
        pq.pop();

        if (currentDist > distances[currentStop]) continue;

        if (distanceGraph.count(currentStop)) {
            for (const auto& neighbor : distanceGraph[currentStop]) {
                int nextStop = neighbor.first;
                int weight = neighbor.second;

                if (distances[currentStop] + weight < distances[nextStop]) {
                    distances[nextStop] = distances[currentStop] + weight;
                    previousStops[nextStop] = currentStop;
                    pq.push({distances[nextStop], nextStop});
                }
            }
        }
    }

    std::vector<int> path;
    int current = targetStopId;
    while (current != startStopId && previousStops.count(current)) {
        path.push_back(current);
        current = previousStops[current];
    }

    if (current == startStopId) {
        path.push_back(startStopId);
        std::reverse(path.begin(), path.end());
        return {distances[targetStopId], path};
    }

    return {-1, {}};
}

std::vector<std::pair<int, std::vector<int>>> TransportSystem::getLinesAndTransfers(const std::vector<int>& path) {
    std::vector<std::pair<int, std::vector<int>>> pathLinesInfo;
    if (path.empty()) return pathLinesInfo;

    int currentLineNo = -1;
    std::vector<int> currentLineStops;

    for (size_t i = 0; i < path.size(); ++i) {
        int currentStopId = path[i];
        const auto& linesAtCurrent = stops[currentStopId].passingLines;

        if (currentLineNo == -1) {
            if (!linesAtCurrent.empty()) {
                currentLineNo = *linesAtCurrent.begin();
            }
            currentLineStops.push_back(currentStopId);
        } else {
            if (linesAtCurrent.count(currentLineNo) == 0) {
                pathLinesInfo.push_back({currentLineNo, currentLineStops});
                currentLineStops.clear();
                currentLineNo = -1;

                if (!linesAtCurrent.empty()) {
                    if (i + 1 < path.size()) {
                        int nextStopId = path[i + 1];
                        const auto& linesAtNext = stops[nextStopId].passingLines;
                        for (int candidate : linesAtCurrent) {
                            if (linesAtNext.count(candidate)) {
                                currentLineNo = candidate;
                                break;
                            }
                        }
                    }
                    if (currentLineNo == -1) currentLineNo = *linesAtCurrent.begin();
                }
            }
            currentLineStops.push_back(currentStopId);
        }
    }
    if (currentLineNo != -1 || !currentLineStops.empty()) {
        pathLinesInfo.push_back({currentLineNo, currentLineStops});
    }

    return pathLinesInfo;
}

void TransportSystem::printJourneyInfo(int passengerId, const std::string& fullName, int startId, int targetId, int totalDistance, const std::vector<int>& path) {
    std::cout << "\n--- Journey Plan for Passenger ID: " << passengerId << " (" << fullName << ") ---" << std::endl;

    if (totalDistance == -1 || path.empty()) {
        std::cout << "No path found between " << startId << " and " << targetId << "." << std::endl;
        return;
    }

    std::cout << "Start: " << stops[startId].name << "\nTarget: " << stops[targetId].name << "\nTotal Distance: " << totalDistance << std::endl;

    auto linesInfo = getLinesAndTransfers(path);
    int transferCount = -1;

    for (const auto& segment : linesInfo) {
        if (segment.first != -1) {
            transferCount++;
            std::cout << "  Take Line " << segment.first << " (" << lines[segment.first].name << ") from "
                      << stops[segment.second.front()].name << " to " << stops[segment.second.back()].name << std::endl;
            lineUsageCounts[segment.first]++;
        }
    }
    std::cout << "Total Transfers: " << std::max(0, transferCount) << "\n------------------------------------------" << std::endl;
}

void TransportSystem::planJourneys(const std::string& passengerListFile) {
    std::ifstream passengersFile(passengerListFile);
    if (!passengersFile.is_open()) {
        std::cerr << "Error: Could not open passengers file: " << passengerListFile << std::endl;
        return;
    }

    std::string lineContent;
    std::getline(passengersFile, lineContent); // Skip header
    while (std::getline(passengersFile, lineContent)) {
        lineContent = trim(lineContent);
        if (lineContent.empty()) continue;

        std::stringstream ss(lineContent);
        int passId, startId, targetId;
        std::string name;

        if (ss >> passId && ss.ignore(256, ',') && std::getline(ss, name, ',') &&
            ss >> startId && ss.ignore(256, ',') && ss >> targetId) {
            name = trim(name);
            auto result = calculateShortestPath(startId, targetId);
            printJourneyInfo(passId, name, startId, targetId, result.first, result.second);
        }
    }
}

void TransportSystem::detectTransferPoints() {
    std::cout << "\n--- Transfer Points ---" << std::endl;
    for (const auto& pair : stops) {
        if (pair.second.passingLines.size() > 1) {
            std::cout << "Stop ID: " << pair.second.id << ", Name: " << pair.second.name << std::endl;
        }
    }
}

void TransportSystem::printMostUsedLines() {
    std::cout << "\n--- Most Used Lines ---" << std::endl;
    std::vector<std::pair<int, int>> sortedLines(lineUsageCounts.begin(), lineUsageCounts.end());
    std::sort(sortedLines.begin(), sortedLines.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    for (const auto& pair : sortedLines) {
        std::cout << "Line ID: " << pair.first << " (" << lines[pair.first].name << "), Usages: " << pair.second << std::endl;
    }
}

void TransportSystem::listAllLines() {
    std::cout << "\n--- All Lines ---" << std::endl;
    for (const auto& pair : lines) {
        std::cout << "Line ID: " << pair.second.number << ", Name: " << pair.second.name << std::endl;
    }
}

void TransportSystem::displayStopsOnLine(int lineNumber) {
    if (lines.count(lineNumber)) {
        std::cout << "\n--- Stops on Line " << lineNumber << " ---" << std::endl;
        for (int stopId : lines[lineNumber].stops) {
            std::cout << stops[stopId].name << " -> ";
        }
        std::cout << "END" << std::endl;
    } else {
        std::cout << "Line " << lineNumber << " not found." << std::endl;
    }
}

void TransportSystem::showMenu() {
    int choice;
    do {
        std::cout << "\n=== TRANSPORTATION SYSTEM MENU ===" << std::endl;
        std::cout << "1. List all lines\n2. Plan Journeys\n3. Search Stop\n4. Display stops on a line\n"
                  << "5. Transfer Points\n6. Most Used Lines\n0. Exit\nChoice: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: listAllLines(); break;
            case 2: planJourneys("data/passengers.txt"); break;
            case 3: {
                int id; std::cout << "Enter Stop ID: "; std::cin >> id;
                searchStop(id); break;
            }
            case 4: {
                int id; std::cout << "Enter Line ID: "; std::cin >> id;
                displayStopsOnLine(id); break;
            }
            case 5: detectTransferPoints(); break;
            case 6: printMostUsedLines(); break;
            case 0: std::cout << "Exiting..." << std::endl; break;
        }
    } while (choice != 0);
}
