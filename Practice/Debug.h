#pragma once
#include <Windows.h>
#include <chrono>
#include <map>
#include <vector>
#include <fstream>
#include <string>
//Can be used in any function to measure the timings,
//and the result is outputed to the console.
#define TIMED NSDebug::Timer _t_(__LINE__,__FILE__,__FUNCTION__)
//Currently can be used inside the classes only where 
//we have timingData map defined.
#define TIMED_DATA NSDebug::Timer _t_(__LINE__,__FILE__,__FUNCTION__,&this->timingData)
namespace NSDebug {
	struct Timer {
		std::chrono::steady_clock::time_point begin;
		std::chrono::steady_clock::time_point end;
		int lineNumber;
		const char * fileName;
		const char * functionName;
		std::map<std::string, std::vector<int>> * timingData = nullptr;
		Timer(int lineNumber,
			const char * fileName,
			const char * functionName) {
			this->lineNumber = lineNumber;
			this->fileName = fileName;
			this->functionName = functionName;
			begin = std::chrono::steady_clock::now();
		}
		Timer(int lineNumber,
			const char * fileName,
			const char * functionName,
			std::map<std::string, std::vector<int>> * timingData) {
			this->lineNumber = lineNumber;
			this->fileName = fileName;
			this->functionName = functionName;
			this->begin = std::chrono::steady_clock::now();
			this->timingData = timingData;
		}
		~Timer() {
			end = std::chrono::steady_clock::now();
			int timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			if (this->timingData != nullptr) {
				std::string s(this->fileName);
				s.append("-");
				s.append(this->functionName);
				//Maybe write our own code to handle this 
				//section of the code as I dont like the 
				//exceptions.
				try {
					std::vector<int> & ref = this->timingData->at(s);
					ref.push_back(timeElapsed);
				}
				catch (std::out_of_range & e) {
					this->timingData->insert(std::pair<std::string, std::vector<int>>(s, { timeElapsed }));
				}
			}
			else {
				char msg[1000];
				sprintf_s(msg, "File %s - Line %d - Function %s - Took %d milliseconds\n",
					this->fileName,
					this->lineNumber,
					this->functionName,
					timeElapsed);
				OutputDebugStringA(msg);
			}
		}
	};
	void WriteTimingDataOut(std::map<std::string, std::vector<int>> * timingData);
}