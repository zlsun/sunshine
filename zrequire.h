#ifndef ZREQUIRE_H
#define ZREQUIRE_H

#include <iostream>
#include <fstream>
#include <cstdlib>

inline void require(bool requirement,
                    const char* msg = "Requirement failed!") {
    if (!requirement) {
        std::cerr << msg << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#define REQUIRE(requirement) require(requirement, "Requirement "#requirement" failed!")


inline void assure(std::ifstream& file,
                   const char* file_name = "") {
    if (!file) {
        std::cerr << "Cann't open file " << file_name << " for reading!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

inline void assure(std::ofstream& file,
                   const char* file_name = "") {
    if (!file) {
        std::cerr << "Cann't open file " << file_name << " for writting!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

#endif // ZREQUIRE_H