#include "Debug.h"
namespace NSDebug {
	void WriteTimingDataOut(std::map<std::string, std::vector<int>> * timingData) {
		std::ofstream file;
		file.open("C:\\Users\\Winny-Banni\\Desktop\\timingData.csv");
		
		std::string s;
		s.append("Function name, Time\n");
		for (auto it = timingData->begin(); it != timingData->end(); it++) {
			s.append(it->first);
			s.append(",");
			std::vector<int> vec = it->second;
			for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
				s.append(std::to_string(*it2));
				s.append(",");
			}
			s.append("\n");
		}
		file << s;
		file.close();
	}
}