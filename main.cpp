#include <regex>
#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>

using namespace std;

class Contact {
private:
    // Private attribute
    string firstName, familyName, phoneNumber = "---", emailAddress = "---";

    static bool isEmailValid(const string &email) {
        // define a regular expression
        const regex pattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");

        // try to match the string with the regular expression
        return regex_match(email, pattern);
    }

public:
    Contact() = default;

    Contact(string firstName, string familyName, string phoneNumber, string emailAddress) {
        setFirstName(move(firstName));
        setFamilyName(move(familyName));
        setPhoneNumber(move(phoneNumber));
        setEmailAddress(move(emailAddress));
    }

    void setFirstName(string firstName) {
        this->firstName = firstName;
    }

    void setFamilyName(string familyName) {
        this->familyName = familyName;
    }

    void setPhoneNumber(string phoneNumber) {
        if (phoneNumber == "---") return;
        for (char const &c: phoneNumber) {
            if (isdigit(c) == 0) {
                cout << "Phone number is invalid !" << endl;
                return;
            }
        }

        if (phoneNumber.find("+98") != string::npos)
            phoneNumber = phoneNumber.replace(0, 3, "0");

        if (phoneNumber.length() != 11) {
            cout << "Phone number is invalid !" << endl;
            return;
        }

        this->phoneNumber = phoneNumber;
    }

    void setEmailAddress(string emailAddress) {
        if (emailAddress == "---") return;
        if (isEmailValid(emailAddress))
            this->emailAddress = emailAddress;

        else
            cout << "Email address is invalid !" << endl;
    }

    const char *getFirstName() {
        return firstName.c_str();
    }

    const char *getFamilyName() {
        return familyName.c_str();
    }

    const char *getPhoneNumber() {
        return phoneNumber.c_str();
    }

    const char *getEmailAddress() {
        return emailAddress.c_str();
    }

    string toString(bool header = false) {
        char buffer[500];
        sprintf(buffer, (char *) "%-15s | %-15s | %-15s | %-15s", getFirstName(),
                getFamilyName(), getPhoneNumber(), getEmailAddress());

        string str = buffer;

        if (header) {
            char buffer2[500];
            sprintf(buffer2, (char *) "%-15s | %-15s | %-15s | %-15s", "First Name",
                    "Family Name", "Phone Number", "Email Address");
            string head = buffer2;

            str = head + "\n" + str;
        }

        return str;
    }
};

class Main {
private:
    vector<Contact> contacts;
    string fileName = "contacts.save";

    void addContact() {
        Contact contact;
        string firstName, familyName, phoneNumber, emailAddress;

        cout << "Enter the first name:" << endl;
        getline(cin, firstName);
        contact.setFirstName(firstName);

        cout << "Enter the family name:" << endl;
        getline(cin, familyName);
        contact.setFamilyName(familyName);

        cout << "Enter the phone number:" << endl;
        getline(cin, phoneNumber);
        contact.setPhoneNumber(phoneNumber);

        cout << "Enter the email address:" << endl;
        getline(cin, emailAddress);
        contact.setEmailAddress(emailAddress);

        contacts.push_back(contact);
        cout << "\nContact added successfully !\n" << endl;
    }

    void removeContact() {
        if (contacts.size() > 0) {
            string index_;
            Contact contact;

            cout << "Choose the contact you want to delete by entering its number:\n";
            showContacts();

            cout << ">" << endl;
            getline(cin, index_);

            try {
                int index = stoi(index_);
                contact = contacts[index - 1];
                contacts.erase(contacts.begin() + index - 1);

                cout << contact.getFirstName() << " " << contact.getFamilyName() << " was deleted successfully !\n"
                     << endl;

            } catch (const std::exception &e) {
                cout << "Wrong command !" << endl << endl;
            }

        } else
            cout << "There is not any contact !" << endl << endl;
    }

    void searchContact() {
        if (contacts.size() > 0) {
            vector<Contact> find;
            string toFind;

            cout << "Enter any detail to find any match in contacts:" << endl;
            getline(cin, toFind);

            for (int i = 0; i < contacts.size(); i++)
                if (contacts[i].toString(false).find(toFind) != string::npos)
                    find.push_back(contacts[i]);

            if (find.size() > 0) {
                cout << "Matching contacts:\n" << endl;
                printf((char *) "-N- | %-15s | %-15s | %-15s | %-15s", "First Name",
                       "Family Name", "Phone Number", "Email Address");
                cout << endl;

                for (int i = 0; i < find.size(); i++)
                    if (find[i].toString(false).find(toFind) != string::npos)
                        cout << i + 1 << "   | " << find[i].toString(false) << endl;

                cout << endl;
            } else
                cout << "No matching contacts where found !" << endl << endl;
        } else
            cout << "There is not any contact !" << endl << endl;
    }

    void showContacts(bool header = false) {
        if (contacts.size() > 0) {
            if (header) {
                printf((char *) "-N- | %-15s | %-15s | %-15s | %-15s", "First Name",
                       "Family Name", "Phone Number", "Email Address");
                cout << endl;
            }

            for (int i = 0; i < contacts.size(); i++) {
                printf((char *) "%-4d | %-15s | %-15s | %-15s | %-15s", i + 1, contacts[i].getFirstName(),
                       contacts[i].getFamilyName(), contacts[i].getPhoneNumber(), contacts[i].getEmailAddress());
                cout << endl;
            }
            cout << endl;
        } else
            cout << "There is not any contact !" << endl << endl;

    }

    void save() {
        if (contacts.size() > 0) {
            ofstream file(fileName);

            char buffer[500];
            sprintf(buffer, (char *) "%-15s | %-15s | %-15s | %-15s", "First Name",
                    "Family Name", "Phone Number", "Email Address");

            file << buffer << endl;
            for (int i = 0; i < contacts.size(); i++) {
                file << contacts[i].toString(false) << endl;
            }
            file.close();

            cout << "Contacts were saved !" << endl << endl;
        } else
            cout << "There is not any contact !" << endl << endl;
    }

    void load() {
        ifstream file(fileName);
        int i = 0;
        string line;
        while (getline(file, line)) {
            if (i == 0) {
                i++;
                continue;
            }

            vector<string> splits = split(" " + removeSpaces(line) + " ", '|');
            Contact newCon = *new Contact(splits[0], splits[1],
                                          splits[2], splits[3]);

            contacts.push_back(newCon);
        }
        file.close();
    }

    string removeSpaces(string str) {
        // n is length of the original string
        int n = str.length();

        // i points to next position to be filled in
        // output string/ j points to next character
        // in the original string
        int i = 0, j = -1;

        // flag that sets to true is space is found
        bool spaceFound = false;

        // Handles leading spaces
        while (++j < n && str[j] == ' ');

        // read all characters of original string
        while (j < n) {
            // if current characters is non-space
            if (str[j] != ' ') {
                // remove preceding spaces before dot,
                // comma & question mark
                if ((str[j] == '.' || str[j] == ',' ||
                     str[j] == '?') && i - 1 >= 0 &&
                    str[i - 1] == ' ')
                    str[i - 1] = str[j++];

                else
                    // copy current character at index i
                    // and increment both i and j
                    str[i++] = str[j++];

                // set space flag to false when any
                // non-space character is found
                spaceFound = false;
            }
                // if current character is a space
            else if (str[j++] == ' ') {
                // If space is encountered for the first
                // time after a word, put one space in the
                // output and set space flag to true
                if (!spaceFound) {
                    str[i++] = ' ';
                    spaceFound = true;
                }
            }
        }

        // Remove trailing spaces
        if (i <= 1)
            str.erase(str.begin() + i, str.end());
        else
            str.erase(str.begin() + i - 1, str.end());

        return str;
    }

    vector<string> split(string str, char char_) {
        string temp = "";
        vector<string> splits;

        for (int i = 0; i < (int) str.size(); i++) {
            if (str[i] != char_)
                temp += str[i];

            else {
                splits.push_back(temp.substr(1, temp.length() - 2));
                temp = "";
            }
        }
        splits.push_back(temp.substr(1, temp.length() - 2));

        return splits;
    }

public:
    Main() {
        load();

        cout << "**/ Welcome to PhoneBook \\**" << endl << endl;
        cout << ">> Choose the commands bellow:" << endl;
        int input;

        while (true) {
            input = menu();

            switch (input) {
                case 1:
                    addContact();
                    continue;

                case 2:
                    removeContact();
                    continue;

                case 3:
                    searchContact();
                    continue;

                case 4:
                    showContacts(true);
                    continue;

                case 5:
                    save();
                    continue;

                case 6:
                    break;

                default:
                    cout << "Wrong command !" << endl << endl;
                    continue;
            }
            break;
        }

        cout << "\nHave a good day !" << endl;
    }

    int menu() {
        cout << "1. Add Contact\n2. Remove Contact\n3. Search Contact\n4. Show Contact\n5. Save To File\n6. Exit\n>"
             << endl;

        string input_;
        getline(cin, input_);

        try {
            int input = stoi(input_);
            if (1 <= input && input <= 6)
                return input;

        } catch (const std::exception &e) {
            return 0;
        }

        return 0;
    }
};

int main() {
    Main m;
    return 0;
}
