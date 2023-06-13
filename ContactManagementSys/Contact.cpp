#include "Contact.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <QJsonDocument>
#include <QJsonArray>

using namespace std;

ContactList::ContactList() {
    head = nullptr;
}

ContactList::~ContactList() {
    Contact* current = head;
    while (current != nullptr) {
        Contact* next = current->next;
        delete current;
        current = next;
    }
}

void ContactList::addContact(string name, string company, string position, string phone, string email) {
    Contact* newContact = new Contact;
    newContact->name = name;
    newContact->company = company;
    newContact->position = position;
    newContact->phone = phone;
    newContact->email = email;
    newContact->next = nullptr;

    if (head == nullptr) {
        head = newContact;
    }
    else {
        Contact* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newContact;
    }
    updateContacts();
}

void ContactList::deleteContact(string name) {
    if (head == nullptr) {
        return;
    }
    if (head->name == name) {
        Contact* temp = head;
        head = head->next;
        delete temp;
        updateContacts();
        return;
    }
    Contact* current = head;
    while (current->next != nullptr) {
        if (current->next->name == name) {
            Contact* temp = current->next;
            current->next = current->next->next;
            delete temp;
            updateContacts();
            return;
        }
        current = current->next;
    }
}

void ContactList::editContact(string name, string company, string position, string phone, string email) {
    Contact* current = head;
    while (current != nullptr) {
        if (current->name == name) {
            current->company = company;
            current->position = position;
            current->phone = phone;
            current->email = email;
            updateContacts();
            return;
        }
        current = current->next;
    }
}

void ContactList::sortByName() {
    if (head == nullptr) {
        return;
    }
    bool swapped;
    Contact* current;
    Contact* last = nullptr;
    do {
        swapped = false;
        current = head;
        while (current->next != last) {
            if (current->name > current->next->name) {
                swap(current->name, current->next->name);
                swap(current->company, current->next->company);
                swap(current->position, current->next->position);
                swap(current->phone, current->next->phone);
                swap(current->email, current->next->email);
                swapped = true;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
    updateContacts();
}

void ContactList::sortByPhone() {
    if (head == nullptr) {
        return;
    }
    bool swapped;
    Contact* current;
    Contact* last = nullptr;
    do {
        swapped = false;
        current = head;
        while (current->next != last) {
            if (current->phone > current->next->phone) {
                swap(current->name, current->next->name);
                swap(current->company, current->next->company);
                swap(current->position, current->next->position);
                swap(current->phone, current->next->phone);
                swap(current->email, current->next->email);
                swapped = true;
            }
            current = current->next;
        }
        last = current;
    } while (swapped);
    updateContacts();
}

void ContactList::printContacts() {
    Contact* current = head;
    while (current != nullptr) {
        cout << "Name: " << current->name << endl;
        cout << "Company: " << current->company << endl;
        cout << "Position: " << current->position << endl;
        cout << "Phone: " << current->phone << endl;
        cout << "Email: " << current->email << endl;
        cout << endl;
        current = current->next;
    }
}

bool ContactList::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file);
    if (!doc.isArray()) {
        return false;
    }
    QJsonArray array = doc.array();
    contacts.clear();
    for (int i = 0; i < array.size(); i++) {
        QJsonObject obj = array[i].toObject();
        Contact contact = Contact::fromJson(obj);
        contacts.push_back(contact);
    }
    updateContacts();
    return true;
}

bool ContactList::saveToFile(const string& filename) {
    QJsonArray array;
    for (const Contact& contact : contacts) {
        QJsonObject obj = contact.toJson();
        array.append(obj);
    }
    QJsonDocument doc(array);
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << doc.toJson().toStdString();
    return true;
}

QJsonObject Contact::toJson() const {
    QJsonObject obj;
    obj["name"] = QString::fromStdString(name);
    obj["company"] = QString::fromStdString(company);
    obj["position"] = QString::fromStdString(position);
    obj["phone"] = QString::fromStdString(phone);
    obj["email"] = QString::fromStdString(email);
    return obj;
}

Contact Contact::fromJson(const QJsonObject& json) {
    Contact contact;
    contact.name = json["name"].toString().toStdString();
    contact.company = json["company"].toString().toStdString();
    contact.position = json["position"].toString().toStdString();
    contact.phone = json["phone"].toString().toStdString();
    contact.email = json["email"].toString().toStdString();
    return contact;
}

void ContactList::updateContacts() {
    Contact* current = head;
    while (current != nullptr) {
        Contact* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    for (const Contact& contact : contacts) {
        addContact(contact.name, contact.company, contact.position, contact.phone, contact.email);
    }
}
