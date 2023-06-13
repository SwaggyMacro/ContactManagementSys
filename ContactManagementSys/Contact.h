#pragma once
#include <string>
#include <vector>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class Contact {
public:
    std::string name;
    std::string company;
    std::string position;
    std::string phone;
    std::string email;
    std::string avatar;
    std::string address;
    Contact* next;

    QJsonObject toJson() const;
    static Contact fromJson(const QJsonObject& json);
};

class ContactList {
public:
    ContactList();

    void addContact(Contact* contact);
    void deleteContact(std::string phone);
    void editContact(std::string phone, Contact* contact);
    void sortByName();
    void sortByPhone();
    void printContacts();
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename);

private:
    Contact* head;

};

