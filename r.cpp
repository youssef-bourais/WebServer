
#include <iostream>
#include <string>
#include <vector>

// Struct to hold location configuration
struct Location {
    std::string path;

    Location(const std::string& p) : path(p) {}
};

// Function to check if a URI matches any location
bool matchLocation(const std::string& uri, const std::vector<Location>& locations, std::string& matchedPath) {
    for (std::vector<Location>::const_iterator it = locations.begin(); it != locations.end(); ++it) {
        if (uri.compare(0, it->path.size(), it->path) == 0) {
            matchedPath = it->path;
            return true;
        }
    }
    return false;
}

int main() {
    // Define some locations
    std::vector<Location> locations;
    locations.push_back(Location("/test/"));
    locations.push_back(Location("/image/"));

    // Example URIs to test
    std::string uris[] = {"/main.cpp", "/test/index.html", "/image/logo.png", "/video.mp4"};

    // Check each URI
    for (int i = 0; i < sizeof(uris) / sizeof(uris[0]); ++i) {
        std::string matchedPath;
        if (matchLocation(uris[i], locations, matchedPath)) {
            std::cout << "URI: " << uris[i] << " matches location: " << matchedPath << std::endl;
        } else {
            std::cout << "URI: " << uris[i] << " does not match any location" << std::endl;
        }
    }

    return 0;
}
