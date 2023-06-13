#include "Contact.h"
#include <fstream>
#include <iostream>

QJsonObject Contact::toJson() const {
    QJsonObject json;
    if (!name.empty()) {
        json["name"] = QString::fromStdString(name);
    }
    if (!company.empty()) {
        json["company"] = QString::fromStdString(company);
    }
    if (!position.empty()) {
        json["position"] = QString::fromStdString(position);
    }
    if (!phone.empty()) {
        json["phone"] = QString::fromStdString(phone);
    }
    if (!email.empty()) {
        json["email"] = QString::fromStdString(email);
    }
    if (!avatar.empty()) {
        json["avatar"] = QString::fromStdString(avatar);
    }
    if (!address.empty()) {
        json["address"] = QString::fromStdString(address);
    }
    return json;
}

Contact Contact::fromJson(const QJsonObject& json) {
    Contact contact;
    contact.name = json["name"].toString().toStdString();
    contact.company = json["company"].toString().toStdString();
    contact.position = json["position"].toString().toStdString();
    contact.phone = json["phone"].toString().toStdString();
    contact.email = json["email"].toString().toStdString();
    contact.avatar = json["avatar"].toString().toStdString();
    contact.address = json["address"].toString().toStdString();
    contact.next = nullptr;
    return contact;
}

ContactList::ContactList() {
    head = nullptr;
}



void ContactList::addContact(Contact* contact) {
    if (head == nullptr) {
        head = contact;
    }
    else {
        Contact* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = contact;
    }
}

void ContactList::deleteContact(std::string phone) {
    if (head == nullptr) {
        return;
    }
    if (head->phone == phone) {
        Contact* temp = head;
        head = head->next;
        delete temp;
    }
    else {
        Contact* current = head;
        while (current->next != nullptr && current->next->phone != phone) {
            current = current->next;
        }
        if (current->next != nullptr) {
            Contact* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }
}

void ContactList::editContact(std::string phone, Contact* contact) {
    Contact* current = head;
    while (current != nullptr && current->phone != phone) {
        current = current->next;
    }
    if (current != nullptr) {
        current->name = contact->name;
        current->company = contact->company;
        current->position = contact->position;
        current->phone = contact->phone;
        current->email = contact->email;
        current->avatar = contact->avatar;
        current->address = contact->address;
    }
}

void ContactList::sortByName() {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    Contact* current = head;
    Contact* nextNode = nullptr;
    bool swapped = false;

    do {
        swapped = false;
        current = head;

        while (current->next != nextNode) {
            if (current->name > current->next->name) {
                std::swap(current->name, current->next->name);
                std::swap(current->company, current->next->company);
                std::swap(current->position, current->next->position);
                std::swap(current->phone, current->next->phone);
                std::swap(current->email, current->next->email);
                std::swap(current->avatar, current->next->avatar);
                std::swap(current->address, current->next->address);
                swapped = true;
            }
            current = current->next;
        }
        nextNode = current;
    } while (swapped);
}

void ContactList::sortByPhone() {
    if (head == nullptr || head->next == nullptr) {
        return;
    }

    Contact* current = head;
    Contact* nextNode = nullptr;
    bool swapped = false;

    do {
        swapped = false;
        current = head;

        while (current->next != nextNode) {
            if (current->phone > current->next->phone) {
                std::swap(current->name, current->next->name);
                std::swap(current->company, current->next->company);
                std::swap(current->position, current->next->position);
                std::swap(current->phone, current->next->phone);
                std::swap(current->email, current->next->email);
                std::swap(current->avatar, current->next->avatar);
                std::swap(current->address, current->next->address);
                swapped = true;
            }
            current = current->next;
        }
        nextNode = current;
    } while (swapped);
}

void ContactList::printContacts() {
    Contact* current = head;
    while (current != nullptr) {
        std::cout << "Name: " << current->name << std::endl;
        std::cout << "Company: " << current->company << std::endl;
        std::cout << "Position: " << current->position << std::endl;
        std::cout << "Phone: " << current->phone << std::endl;
        std::cout << "Email: " << current->email << std::endl;
        std::cout << "Avatar: " << current->avatar << std::endl;
        std::cout << "Address: " << current->address << std::endl;
        std::cout << std::endl;
        current = current->next;
    }
}

bool ContactList::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    std::string fileData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(fileData).toUtf8());
    if (!jsonDoc.isArray()) {
        return false;
    }
    QJsonArray jsonArray = jsonDoc.array();
    for (const QJsonValue& jsonValue : jsonArray) {
        if (!jsonValue.isObject()) {
            continue;
        }
        QJsonObject jsonObject = jsonValue.toObject();
        Contact* contact = new Contact(Contact::fromJson(jsonObject));
        addContact(contact);
    }
    file.close();
    return true;
}

bool ContactList::saveToFile(const std::string& filename) {
    QJsonArray jsonArray;
    Contact* current = head;
    while (current != nullptr) {
        jsonArray.append(current->toJson());
        current = current->next;
    }
    QJsonDocument jsonDoc(jsonArray);
    QByteArray byteArray = jsonDoc.toJson();
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << byteArray.toStdString();
    file.close();
    return true;
}

Contact* ContactList::getHead() {
	return head;
}