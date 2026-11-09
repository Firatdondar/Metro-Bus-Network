#ifndef TRANSPORT_SYSTEM_H
#define TRANSPORT_SYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include <queue>
#include <algorithm>
#include <set>
#include <fstream>
#include <sstream>

class Stop {
public:
    int id;
    std::string name;
    std::set<int> passingLines;

    Stop(int id = 0, const std::string& name = "") : id(id), name(name) {}
};

class Line {
public:
    int number;
    std::string name;
    std::vector<int> stops;

    Line(int no = 0, const std::string& name = "") : number(no), name(name) {}
};

class TransportSystem {
private:
    std::map<int, Stop> stops;
    std::map<std::string, int> stopNameToId;
    std::map<int, Line> lines;
    std::map<int, std::map<int, int>> distanceGraph;
    std::map<int, int> lineUsageCounts;

    static std::string trim(const std::string& str);

public:
    TransportSystem() = default;
    ~TransportSystem() = default;

    void createGraph(const std::string& stopsFile, const std::string& distancesFile, const std::string& linesFile);
    void searchStop(int stopId);
    std::pair<int, std::vector<int>> calculateShortestPath(int startStopId, int targetStopId);
    std::vector<std::pair<int, std::vector<int>>> getLinesAndTransfers(const std::vector<int>& path);
    void printJourneyInfo(int passengerId, const std::string& fullName, int startId, int targetId, int totalDistance, const std::vector<int>& path);
    void planJourneys(const std::string& passengerListFile);
    void detectTransferPoints();
    void printMostUsedLines();
    void listAllLines();
    void displayStopsOnLine(int lineNumber);
    void showMenu();
};

#endif // TRANSPORT_SYSTEM_H
