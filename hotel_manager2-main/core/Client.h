
#ifndef HOTEL_MANAGAR_CLIENT_H
#define HOTEL_MANAGAR_CLIENT_H

#include <string>
#include <vector>

class Client {
private:
    int id;
    std::string lastName;
    std::string firstName;
    std::string patronymic;
    std::string phone;
    std::string email;

public:
    Client(int clientId, std::string last, std::string first,
           std::string patron, std::string clientPhone, std::string clientEmail);

    int getId() const;
    std::string getLastName() const;
    std::string getFirstName() const;
    std::string getPatronymic() const;
    std::string getPhone() const;
    std::string getEmail() const;
    std::string getFullName() const;

    static void saveToFile(const std::vector<Client>& clients, const std::string& filename = "clients.txt");
    static std::vector<Client> loadFromFile(const std::string& filename = "clients.txt");
};

#endif //HOTEL_MANAGAR_CLIENT_H
