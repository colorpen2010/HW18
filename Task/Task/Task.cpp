#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

using namespace std;

class Contact
{
private:
    char* fullName;
    string homePhone;
    string workPhone;
    string mobilePhone;
    string info;

public:
    Contact() : fullName(nullptr), homePhone(""), workPhone(""), mobilePhone(""), info("")
    {
        setName("");
    }

    Contact(string name, string home, string work, string mobile, string extra)
        : fullName(nullptr), homePhone(home), workPhone(work), mobilePhone(mobile), info(extra)
    {
        setName(name);
    }

    Contact(const Contact& other)
        : fullName(nullptr), homePhone(other.homePhone), workPhone(other.workPhone),
          mobilePhone(other.mobilePhone), info(other.info)
    {
        setName(other.getName());
    }

    Contact& operator=(const Contact& other)
    {
        if (this != &other)
        {
            homePhone = other.homePhone;
            workPhone = other.workPhone;
            mobilePhone = other.mobilePhone;
            info = other.info;
            setName(other.getName());
        }
        return *this;
    }

    inline string getName() const { return fullName; }
    inline string getHomePhone() const { return homePhone; }
    inline string getWorkPhone() const { return workPhone; }
    inline string getMobilePhone() const { return mobilePhone; }
    inline string getInfo() const { return info; }

    void setName(const string& name)
    {
        delete[] fullName;
        fullName = new char[name.length() + 1];
        strcpy_s(fullName, name.length() + 1, name.c_str());
    }

    void setPhones(const string& home, const string& work, const string& mobile)
    {
        homePhone = home;
        workPhone = work;
        mobilePhone = mobile;
    }

    void setInfo(const string& extra) { info = extra; }

    void show() const
    {
        cout << "ПІБ: " << fullName << "\n"
             << "Домашній телефон: " << homePhone << "\n"
             << "Робочий телефон: " << workPhone << "\n"
             << "Мобільний телефон: " << mobilePhone << "\n"
             << "Додаткова інформація: " << info << "\n";
    }

    ~Contact()
    {
        delete[] fullName;
    }
};

class PhoneBook
{
private:
    vector<Contact> contacts;

public:
    void addContact()
    {
        string name, home, work, mobile, extra;
        cout << "Введіть ПІБ: "; getline(cin, name);
        cout << "Домашній телефон: "; getline(cin, home);
        cout << "Робочий телефон: "; getline(cin, work);
        cout << "Мобільний телефон: "; getline(cin, mobile);
        cout << "Додаткова інформація: "; getline(cin, extra);
        contacts.push_back(Contact(name, home, work, mobile, extra));
        cout << "Абонента додано.\n";
    }

    void showAll() const
    {
        if (contacts.empty())
        {
            cout << "Телефонна книга порожня.\n";
            return;
        }
        for (size_t i = 0; i < contacts.size(); i++)
        {
            cout << "\n--- Абонент " << i + 1 << " ---\n";
            contacts[i].show();
        }
    }

    void searchContact() const
    {
        string name;
        cout << "Введіть ПІБ для пошуку: "; getline(cin, name);
        bool found = false;
        for (size_t i = 0; i < contacts.size(); i++)
        {
            if (contacts[i].getName() == name)
            {
                contacts[i].show();
                found = true;
            }
        }
        if (!found) cout << "Абонента не знайдено.\n";
    }

    void deleteContact()
    {
        string name;
        cout << "Введіть ПІБ абонента для видалення: "; getline(cin, name);
        for (size_t i = 0; i < contacts.size(); i++)
        {
            if (contacts[i].getName() == name)
            {
                contacts.erase(contacts.begin() + i);
                cout << "Абонента видалено.\n";
                return;
            }
        }
        cout << "Абонента не знайдено.\n";
    }

    void saveToFile() const
    {
        ofstream file("phonebook.txt");
        if (!file)
        {
            cout << "Не вдалося відкрити файл для запису.\n";
            return;
        }
        file << contacts.size() << '\n';
        for (size_t i = 0; i < contacts.size(); i++)
        {
            file << contacts[i].getName() << '\n'
                 << contacts[i].getHomePhone() << '\n'
                 << contacts[i].getWorkPhone() << '\n'
                 << contacts[i].getMobilePhone() << '\n'
                 << contacts[i].getInfo() << '\n';
        }
        cout << "Дані збережено у файлі phonebook.txt.\n";
    }

    void loadFromFile()
    {
        ifstream file("phonebook.txt");
        if (!file)
        {
            cout << "Файл phonebook.txt не знайдено.\n";
            return;
        }
        size_t count;
        if (!(file >> count))
        {
            cout << "Файл має неправильний формат.\n";
            return;
        }
        file.ignore();
        vector<Contact> loaded;
        for (size_t i = 0; i < count; i++)
        {
            string name, home, work, mobile, extra;
            if (!getline(file, name) || !getline(file, home) || !getline(file, work) ||
                !getline(file, mobile) || !getline(file, extra))
            {
                cout << "Не вдалося прочитати всі дані з файлу.\n";
                return;
            }
            loaded.push_back(Contact(name, home, work, mobile, extra));
        }
        contacts = loaded;
        cout << "Дані завантажено з файлу phonebook.txt.\n";
    }
};

int main()
{
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    PhoneBook book;
    int choice;
    do
    {
        cout << "\n=== ТЕЛЕФОННА КНИГА ===\n"
             << "1. Додати абонента\n"
             << "2. Видалити абонента\n"
             << "3. Знайти абонента за ПІБ\n"
             << "4. Показати всіх абонентів\n"
             << "5. Зберегти у файл\n"
             << "6. Завантажити з файлу\n"
             << "0. Вихід\n"
             << "Ваш вибір: ";
        if (!(cin >> choice))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Введіть номер пункту меню.\n";
            choice = -1;
            continue;
        }
        cin.ignore(10000, '\n');

        switch (choice)
        {
        case 1: book.addContact(); break;
        case 2: book.deleteContact(); break;
        case 3: book.searchContact(); break;
        case 4: book.showAll(); break;
        case 5: book.saveToFile(); break;
        case 6: book.loadFromFile(); break;
        case 0: cout << "До побачення!\n"; break;
        default: cout << "Невірний пункт меню.\n";
        }
    } while (choice != 0);

    return 0;
}
