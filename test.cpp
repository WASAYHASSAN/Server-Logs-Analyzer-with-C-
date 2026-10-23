#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map> 
#include <vector>
using namespace std;

class Parse {
    private:
        string Filename;
        
        string normalize(string endpoint){
        	 
        	size_t markPosition = endpoint.find("?");
        	if (markPosition != string::npos){
    			endpoint = endpoint.substr(0, markPosition);
			}
			size_t idPosition = endpoint.find("usr_");
			
			if (idPosition != string::npos){
				size_t slashPosition = endpoint.find("/", idPosition);
				if (idPosition != string::npos && slashPosition != string::npos){
					endpoint = endpoint.replace(idPosition, slashPosition - idPosition, "{id}");
				}
			}
			
			return endpoint;
		}
		
        
    public:
        Parse(string filename) {
            Filename = filename;    
        }
        
        void read() {
            fstream file;
            file.open(Filename, ios::in);
            
            if (file.is_open()) {
                string line;
                int linecount = 0;
                int errorCount = 0;
                int warningCount = 0;
                int infoCount = 0;
                int debugCount = 0;
                map<string, int> endpointCounts;
                
                
                vector<string> mostFrequentEndpoints;
                int maxErrors = 0;
                
                while (getline(file, line)) {
                    linecount++;
                    
                    bool isErrorLine = false;
                    string extractedEndpoint = "";
                    
                    if (line.find("\"level\": \"ERROR\"") != string::npos) {
                        errorCount++;
                        isErrorLine = true;
                        
                        string endpointKey = "\"endpoint\": \"";
                        size_t startPos = line.find(endpointKey);
                    
                        if (startPos != string::npos) {
                            startPos += endpointKey.length();
                            size_t endPos = line.find("\"", startPos);
                    
                            if (endPos != string::npos) {
                                extractedEndpoint = line.substr(startPos, endPos - startPos);
                            }
                        }
                    }
                    else if (line.find("\"level\": \"WARN\"") != string::npos) {
                        warningCount++;
                    }
                    else if (line.find("\"level\": \"INFO\"") != string::npos) {
                        infoCount++;
                    }
                    else if (line.find("\"level\": \"DEBUG\"") != string::npos) {
                        debugCount++;
                    }
                    if (isErrorLine == true && extractedEndpoint != "") {
                    	extractedEndpoint = normalize(extractedEndpoint);
                        endpointCounts[extractedEndpoint]++;
                        int currentEndpointCount = endpointCounts[extractedEndpoint];
                        if (currentEndpointCount > maxErrors) {
                            maxErrors = currentEndpointCount;
                            mostFrequentEndpoints.clear(); 
                            mostFrequentEndpoints.push_back(extractedEndpoint); 
                        }
                        else if (currentEndpointCount == maxErrors) {
                            mostFrequentEndpoints.push_back(extractedEndpoint); 
                        }
                    }
                }
                
                
                cout << "\n====== LOG ANALYSIS REPORT ======\n";
                cout << "Total lines processed : " << linecount << endl;
                cout << "Errors caught         : " << errorCount << endl;
                cout << "Warnings caught       : " << warningCount << endl;
                cout << "Info statements caught: " << infoCount << endl;
                cout << "Debug entries caught  : " << debugCount << endl;
                
                cout << "\n--- Endpoint Error Breakdown ---\n";
                for (map<string, int>::iterator it = endpointCounts.begin(); it != endpointCounts.end(); ++it) {
                    cout << "Endpoint: " << it->first << " | Failures: " << it->second << endl;
                }
                cout << "--------------------------------\n";
                
                
                if (maxErrors > 0) {
                    cout << "\nMost frequent error endpoint(s) (" << maxErrors << " times each):" << endl;
                    for (size_t i = 0; i < mostFrequentEndpoints.size(); i++) {
                        cout << " -> " << mostFrequentEndpoints[i] << endl;
                    }
                }
                cout << "=================================\n";
            }
            else {
                cout << "Failed to open file." << endl;
            }
        }
};

int main() {
    
    Parse file("server_logs.txt");
    file.read();
    return 0;
}
