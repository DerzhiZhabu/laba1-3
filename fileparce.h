#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

template <typename T>
void find_values_in_file(const std::string& file_path, const std::string& var, T& values) {

    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error("No such file");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find(var + " = ") == 0) {
            std::istringstream iss(line.substr(var.length() + 3)); // +3 для пропуска " = "
            std::string value;
            while (iss >> value) {
                values.push(value);
            }
            break;
        }
    }

    file.close();
}

template <typename T>
void find_values_in_file_ARR(const std::string& file_path, const std::string& var, T& values) {

    std::ifstream file(file_path);

    if (!file.is_open()) {
        throw std::runtime_error("No such file");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.find(var + " = ") == 0) {
            std::istringstream iss(line.substr(var.length() + 3)); // +3 для пропуска " = "
            std::string value;
            int i = -1;
            while (iss >> value) {
                i++;
                if (value == "'"){
                    continue;
                }
                values.set(value, i);
            }
            break;
        }
    }

    file.close();
}

template <typename T>
void rewrite_values_in_file_ARR(const std::string& file_path, const std::string& var, T& new_values) {
    std::ifstream file(file_path);
    std::ofstream temp_file("temp.txt");

    if (!file.is_open() || !temp_file.is_open()) {
        throw std::runtime_error("No such file");
    }

    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find(var + " = ") == 0) {
            temp_file << var << " = ";
            int o = new_values.length;
            for (int i = 0; i < o; i++) {
                if (!new_values.get(i).state) new_values.set("\'", i);
                temp_file << new_values[i] << " ";
            }
            temp_file << std::endl;
            found = true;
        } else {
            temp_file << line << std::endl;
        }
    }

    if (!found) {
        temp_file << var << " = ";
        int o = new_values.length;
        for (int i = 0; i < o; i++) {
            if (!new_values.get(i).state) new_values.set("\'", i);
            temp_file << new_values[i] << " ";
        }
        temp_file << std::endl;
    }

    file.close();
    temp_file.close();

    std::remove(file_path.c_str());
    std::rename("temp.txt", file_path.c_str());
}

template <typename T>
void rewrite_values_in_file(const std::string& file_path, const std::string& var, T& new_values) {
    std::ifstream file(file_path);
    std::ofstream temp_file("temp.txt");

    if (!file.is_open() || !temp_file.is_open()) {
        throw std::runtime_error("No such file");
    }

    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find(var + " = ") == 0) {
            temp_file << var << " = ";
            for (int i = 0; i < new_values.length; i++) {
                temp_file << new_values[i] << " ";
            }
            temp_file << std::endl;
            found = true;
        } else {
            temp_file << line << std::endl;
        }
    }

    if (!found) {
        temp_file << var << " = ";
        for (int i = 0; i < new_values.length; i++) {
            temp_file << new_values[i] << " ";
        }
        temp_file << std::endl;
    }

    file.close();
    temp_file.close();

    std::remove(file_path.c_str());
    std::rename("temp.txt", file_path.c_str());
}

template <typename T>
void rewrite_values_in_file_Odn(const std::string& file_path, const std::string& var, T& new_values) {
    std::ifstream file(file_path);
    std::ofstream temp_file("temp.txt");

    if (!file.is_open() || !temp_file.is_open()) {
        throw std::runtime_error("No such file");
    }

    std::string line;
    bool found = false;

    while (std::getline(file, line)) {
        if (line.find(var + " = ") == 0) {
            temp_file << var << " = ";
            while (new_values.length != 0) {
                temp_file << new_values.pop() << " ";
            }
            temp_file << std::endl;
            found = true;
        } else {
            temp_file << line << std::endl;
        }
    }

    if (!found) {
        temp_file << var << " = ";
        while (new_values.length != 0) {
            temp_file << new_values.pop() << " ";
        }
        temp_file << std::endl;
    }

    file.close();
    temp_file.close();

    std::remove(file_path.c_str());
    std::rename("temp.txt", file_path.c_str());
}