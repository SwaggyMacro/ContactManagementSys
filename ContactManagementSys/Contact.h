#pragma once
#include <string>
#include <vector>
#include <QJsonObject>

class Contact {
public:
    std::string name;
    std::string company;
    std::string position;
    std::string phone;
    std::string email;
    Contact* next;

    QJsonObject toJson() const;
    static Contact fromJson(const QJsonObject& json);
};

class ContactList {
public:
    ContactList();
    ~ContactList();

    void addContact(std::string name, std::string company, std::string position, std::string phone, std::string email);
    void deleteContact(std::string name);
    void editContact(std::string name, std::string company, std::string position, std::string phone, std::string email);
    void sortByName();
    void sortByPhone();
    void printContacts();
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);

private:
    Contact* head;
    std::vector<Contact> contacts;

    void updateContacts();
};

