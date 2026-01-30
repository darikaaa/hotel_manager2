

#include "Client.h"

#include <fstream>
#include <iostream>
#include <sstream>

Client::Client(int clientId, std::string last, std::string first,
               std::string patron, std::string clientPhone, std::string clientEmail)
{
    id = clientId;
    lastName = last;
    firstName = first;
    patronymic = patron;
    phone =clientPhone;
    email = clientEmail;

    if (clientId <= 0) {
        throw "Client ID must be positive";
    }
    if (last.empty() || first.empty()) {
        throw "Last name and first name cannot be empty";
    }
    if (phone.empty()) {
        throw "Phone cannot be empty";
    }
    if (email.empty()){
        throw "Email cannotbe empty";
    }
}

int Client::getId() const { return id; }
std::string Client::getLastName() const { return lastName; }
std::string Client::getFirstName() const { return firstName; }
std::string Client::getPatronymic() const { return patronymic; }
std::string Client::getPhone() const { return phone; }
std::string Client::getEmail() const { return email; }
std::string Client::getFullName() const {
    return lastName + " " + firstName + (patronymic.empty() ? "" : " " + patronymic);
}

void Client::saveToFile(const std::vector<Client>& clients, const std::string& filename) {
    std::ofstream file(filename);

    if (!file) {
        throw "File is not opened";
    }

    for (const Client& client : clients) {
        file << client.getId() << "|"
             << client.getLastName() << "|"
             << client.getFirstName() << "|"
             << client.getPatronymic() << "|"
             << client.getPhone() << "|"
             << client.getEmail() << "\n";
    }

    file.close();
}

std::vector<Client> Client::loadFromFile(const std::string& filename) {
    std::vector<Client> clients;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw "Cannot open file";
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::string parts[6];
        std::stringstream ss(line);

        for (int i = 0; i < 6; i++) {
            std::getline(ss, parts[i], '|');
        }

        try {
            Client client(
                    std::stoi(parts[0]),
                    parts[1],
                    parts[2],
                    parts[3],
                    parts[4],
                    parts[5]
            );

            clients.push_back(client);
        }
        catch (...) {
            std::cout << "Error in line: " << line << std::endl;
        }
    }

    file.close();
    return clients;
}