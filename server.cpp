// server.cpp - Web server implementation
#include "student.h"
#include <winsock2.h>
#include <sstream>
#include <algorithm>
#include <conio.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

string getJsonValue(const string& json, const string& key) {
    string searchKey = "\"" + key + "\"";
    size_t keyPos = json.find(searchKey);
    if (keyPos == string::npos) return "";
    
    size_t colonPos = json.find(":", keyPos);
    if (colonPos == string::npos) return "";
    
    colonPos++;
    while (colonPos < json.length() && (json[colonPos] == ' ' || json[colonPos] == '\t')) {
        colonPos++;
    }
    
    if (json[colonPos] == '"') {
        colonPos++;
        size_t endQuote = json.find("\"", colonPos);
        if (endQuote != string::npos) {
            return json.substr(colonPos, endQuote - colonPos);
        }
    } else {
        size_t commaPos = json.find(",", colonPos);
        size_t bracePos = json.find("}", colonPos);
        size_t endPos = min(commaPos, bracePos);
        if (endPos != string::npos) {
            return json.substr(colonPos, endPos - colonPos);
        }
    }
    return "";
}

void sendResponse(SOCKET client, int statusCode, string contentType, string body) {
    stringstream response;
    response << "HTTP/1.1 " << statusCode;
    if (statusCode == 200) response << " OK";
    else if (statusCode == 404) response << " Not Found";
    else if (statusCode == 400) response << " Bad Request";
    response << "\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Content-Length: " << body.length() << "\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n";
    response << "Access-Control-Allow-Headers: Content-Type\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;
    
    string resp = response.str();
    send(client, resp.c_str(), resp.length(), 0);
}

void serveStaticFile(SOCKET client, string path) {
    if (path[0] == '/') path = path.substr(1);
    if (path.empty()) path = "index.html";
    
    FILE *fp = fopen(path.c_str(), "rb");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        
        char *buffer = new char[size + 1];
        fread(buffer, 1, size, fp);
        buffer[size] = '\0';
        fclose(fp);
        
        string contentType = "text/html";
        sendResponse(client, 200, contentType, string(buffer));
        delete[] buffer;
    } else {
        sendResponse(client, 404, "text/html", "<h1>404 - File Not Found</h1>");
    }
}

void handleAPI(SOCKET client, string method, string path, string body) {
    if (method == "GET" && path == "/api/students") {
        string json = "[";
        for (size_t i = 0; i < students.size(); i++) {
            if (i > 0) json += ",";
            json += students[i].toJSON();
        }
        json += "]";
        sendResponse(client, 200, "application/json", json);
    }
    else if (method == "GET" && path.find("/api/students/") == 0) {
        int roll = atoi(path.substr(14).c_str());
        for (auto &s : students) {
            if (s.getRoll() == roll) {
                sendResponse(client, 200, "application/json", s.toJSON());
                return;
            }
        }
        sendResponse(client, 404, "application/json", "{\"error\":\"Student not found\"}");
    }
    else if (method == "POST" && path == "/api/students") {
        int roll = atoi(getJsonValue(body, "roll").c_str());
        string name = getJsonValue(body, "name");
        float ps = atof(getJsonValue(body, "ps").c_str());
        float daa = atof(getJsonValue(body, "daa").c_str());
        float dbms = atof(getJsonValue(body, "dbms").c_str());
        float oops = atof(getJsonValue(body, "oops").c_str());
        
        // Check duplicate
        for (auto &s : students) {
            if (s.getRoll() == roll) {
                sendResponse(client, 400, "application/json", "{\"success\":false,\"message\":\"Duplicate roll number\"}");
                return;
            }
        }
        
        Student newStudent;
        newStudent.setRoll(roll);      // FIXED: Set roll number
        newStudent.setName(name);
        newStudent.setPS(ps);
        newStudent.setDAA(daa);
        newStudent.setDBMS(dbms);
        newStudent.setOOPs(oops);
        students.push_back(newStudent);
        saveToFile();
        sendResponse(client, 200, "application/json", "{\"success\":true,\"message\":\"Student added successfully\"}");
    }
    else if (method == "PUT" && path.find("/api/students/") == 0) {
        int roll = atoi(path.substr(14).c_str());
        string name = getJsonValue(body, "name");
        float ps = atof(getJsonValue(body, "ps").c_str());
        float daa = atof(getJsonValue(body, "daa").c_str());
        float dbms = atof(getJsonValue(body, "dbms").c_str());
        float oops = atof(getJsonValue(body, "oops").c_str());
        
        for (auto &s : students) {
            if (s.getRoll() == roll) {
                s.setName(name);
                s.setPS(ps);
                s.setDAA(daa);
                s.setDBMS(dbms);
                s.setOOPs(oops);
                saveToFile();
                sendResponse(client, 200, "application/json", "{\"success\":true,\"message\":\"Student updated successfully\"}");
                return;
            }
        }
        sendResponse(client, 404, "application/json", "{\"success\":false,\"message\":\"Student not found\"}");
    }
    else if (method == "DELETE" && path.find("/api/students/") == 0) {
        int roll = atoi(path.substr(14).c_str());
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->getRoll() == roll) {
                students.erase(it);
                saveToFile();
                sendResponse(client, 200, "application/json", "{\"success\":true,\"message\":\"Student deleted successfully\"}");
                return;
            }
        }
        sendResponse(client, 404, "application/json", "{\"success\":false,\"message\":\"Student not found\"}");
    }
    else if (method == "POST" && path == "/api/sort") {
        sort(students.begin(), students.end(), [](Student &a, Student &b) {
            return a.getPercentage() > b.getPercentage();
        });
        saveToFile();
        sendResponse(client, 200, "application/json", "{\"success\":true,\"message\":\"Sorted by percentage\"}");
    }
    else if (method == "OPTIONS") {
        sendResponse(client, 200, "text/plain", "");
    }
    else {
        sendResponse(client, 404, "application/json", "{\"error\":\"API endpoint not found\"}");
    }
}

void runWebServerMode() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        cout << "WSAStartup failed\n";
        return;
    }
    
    SOCKET server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == INVALID_SOCKET) {
        cout << "Socket creation failed\n";
        WSACleanup();
        return;
    }
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5050);
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (bind(server, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Bind failed. Port 5050 might be in use.\n";
        closesocket(server);
        WSACleanup();
        return;
    }
    
    listen(server, 10);
    
    cout << "\n========================================\n";
    cout << "WEB SERVER MODE\n";
    cout << "========================================\n";
    cout << "Server running at: http://localhost:5050\n";
    cout << "Data file: " << DATA_FILE << "\n";
    cout << "Total students loaded: " << students.size() << "\n";
    cout << "========================================\n";
    cout << "Press 'q' and then Enter to stop server and return to menu\n\n";
    
    bool running = true;
    
    while (running) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server, &readfds);
        
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        
        int activity = select(0, &readfds, NULL, NULL, &tv);
        
        if (activity > 0 && FD_ISSET(server, &readfds)) {
            SOCKET client = accept(server, NULL, NULL);
            if (client != INVALID_SOCKET) {
                char buffer[32768] = {0};
                recv(client, buffer, sizeof(buffer) - 1, 0);
                
                string request(buffer);
                string method, path, version;
                stringstream reqStream(request);
                reqStream >> method >> path >> version;
                
                size_t bodyPos = request.find("\r\n\r\n");
                string body;
                if (bodyPos != string::npos) {
                    body = request.substr(bodyPos + 4);
                }
                
                if (path.find("/api/") == 0) {
                    handleAPI(client, method, path, body);
                } else {
                    if (path == "/") path = "/index.html";
                    serveStaticFile(client, path);
                }
                
                closesocket(client);
            }
        }
        
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'q' || ch == 'Q') {
                cout << "\nStopping web server...\n";
                running = false;
            }
        }
    }
    
    closesocket(server);
    WSACleanup();
}