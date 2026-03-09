### 3. Refactor Edilmiş Kod Yapısı (Başlangıç)

Tüm değişken isimlerini İngilizceye çevirdim ve `include` yollarını düzelttim.

**include/transport_system.h**
```cpp
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
    int stop_id;
    std::string stop_name;
    std::set<int> lines_passing_through;
    Stop(int id, std::string name) : stop_id(id), stop_name(name) {}
};

class TransportSystem {
private:
    // Kendi veri yapılarını buraya taşıyacaksın
public:
    TransportSystem();
    ~TransportSystem();
    
    void createGraph(const std::string& stopsFile, const std::string& distancesFile, const std::string& linesFile);
    void showMenu();
    // Diğer fonksiyonlarını buraya ekle
};

#endif