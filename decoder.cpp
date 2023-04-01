#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>

using namespace std;

// char flag[] = "iamasecretflag";
string d = "b_d3fu";
string b = "er{B";
string c = "0m";
string a = "ctcyb";
string e = "s3d}";

void win(){
    std::cout << "Success! Decryption Key: " + a + b + c + d + e << endl;
}

std::string setFlag(std::string input) {
    // XOR encode the input string by 0x30
    for (char& c : input) {
        c ^= 0x30;
    }

    // Set the flag value based on the result
    std::string flag = input;
    // Remove XOR '\n' from flag
    if (!flag.empty() && flag[flag.length()-1] == ':'){
        flag.erase(flag.length()-1);
    }
    return flag;
}

bool check_magic_value(std::string input, std::string flag) {
    cout << "++++++++"<<endl;
    cout << "Checking value against flag..." <<endl;
    cout << input<<endl;
    // cout << flag<<endl;
    cout << "++++++++"<<endl;

    // Compare the result against the magic value
    return input == flag;
}

void printWarningBanner() {
  cout << R"(   _____ _______    _____      _                  _____                _               __ 
  / ____|__   __|  / ____|    | |                / ____|              | |             /_ |
 | |       | |    | |    _   _| |__   ___ _ __  | |     _ __ __ _  ___| | _____ _ __   | |
 | |       | |    | |   | | | | '_ \ / _ \ '__| | |    | '__/ _` |/ __| |/ / _ \ '__|  | |
 | |____   | |    | |___| |_| | |_) |  __/ |    | |____| | | (_| | (__|   <  __/ |     | |
  \_____|  |_|     \_____\__, |_.__/ \___|_|     \_____|_|  \__,_|\___|_|\_\___|_|     |_|
                          __/ |                                                           
                         |___/                                                            )"
     << '\n';
  cout << "Warning, all of your files are encrypted (not really lol). Send me 50 BTC or else..." << endl;
  cout << "A unique string is available @ http://ctf.ctcyber.io" <<endl;
  cout << "Will send instructions on how to turn this into decryption key when payment is received"<<endl;
  cout << endl;
  
}

const int BUFFER_SIZE = 4096;

std::string get_url_content(const std::string& url) {
    // std::string url = url;
    if (url != "http://ctf.ctcyber.io"){
        cout << "Haha nice try, only valid key is available at http://ctf.ctcyber.io" <<endl;
        exit(0);
    }
    std::string hostname, path;
    size_t start = url.find("://");
    if (start != std::string::npos) {
        start += 3;
        size_t end = url.find('/', start);
        if (end != std::string::npos) {
            hostname = url.substr(start, end - start);
            path = url.substr(end);
        } else {
            hostname = url.substr(start);
            path = "/";
        }
    } else {
        size_t end = url.find('/');
        if (end != std::string::npos) {
            hostname = url.substr(0, end);
            path = url.substr(end);
        } else {
            hostname = url;
            path = "/";
        }
    }

    // Open a TCP connection to the web server
    struct hostent* host = gethostbyname(hostname.c_str());
    if (!host) {
        std::cerr << "Failed to resolve hostname\n";
        exit(1);
        return "Failed to resolve hostname\n";
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Failed to create socket\n";
        exit(2);
        return "Failed to create socket\n";
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    server_addr.sin_addr = *((struct in_addr*)host->h_addr);
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to server\n";
        exit(3);
        return "Failed to connect to server\n";
    }

    // Send an HTTP GET request for the specified path
    std::string request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: " + hostname + "\r\n";
    request += "Connection: close\r\n\r\n";
    if (send(sock, request.c_str(), request.size(), 0) < 0) {
        std::cerr << "Failed to send request\n";
        exit(4);
        return "Failed to send request\n";
    }

    char buffer[BUFFER_SIZE];
    std::string response = "";
    bool headers_done = false;
    while (true) {
        ssize_t count = recv(sock, buffer, BUFFER_SIZE, 0);
        if (count < 0) {
            std::cerr << "Error reading response" << std::endl;
            exit(5);

            return "";
        }
        if (count == 0) {
            break;
        }
        response.append(buffer, count);

        // Check if we have reached the end of the headers
        size_t pos = response.find("\r\n\r\n");
        if (!headers_done && pos != std::string::npos) {
            headers_done = true;
            response = response.substr(pos + 4);
        }
    }

    // Close the socket
    close(sock);
    return response;
}
inline unsigned int to_uint(char ch)
{
    // EDIT: multi-cast fix as per David Hammen's comment
    return static_cast<unsigned int>(static_cast<unsigned char>(ch));
}
int main(int argc, char* argv[]) {
    printWarningBanner();
    if (argc != 2) {
        std::cerr << "[+] Usage: " << argv[0] << " <url>\n";
        return 1;
    }
    string output = get_url_content(argv[1]);  
    string flag = setFlag(output);
    cout << "Decryption string: " + output << endl;
    std::string input;
    std::cout << "Enter your decryption key: ";
    std::getline(std::cin, input);
    if (check_magic_value(input, flag)) {
        //std::cout << "Decryption key: ctcyber{Bomb_Defus3d}" << endl;
        win();
    } else{
        cout << "Sorry, decryption unsuccessful..." << endl;
    }
    return 0;
}

