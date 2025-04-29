#pragma once
#include "sha1.h"
#include <string>

#define SIZE 10
#define LOGINLENGTH 10

/*ЗАДАЧА: Модернизируйте модель чата из задания 15.4.2 так, чтобы логин и хеш от пароля теперь хранились в хеш-таблице. 
То есть логин является ключом, хеш — значением, а вместе они составляются пару. 
Хеш-функцию для хеш-таблицы составьте методом умножения. Хеш-таблицу реализуйте методом квадратичного пробирования.*/

class HashTable         // ХТ
{
public:
    HashTable();                                                                    // конструктор по умолчанию
    ~HashTable();                                                                   // деструктор
    void reg(char _login[LOGINLENGTH], char _pass[], int pass_length);              // метод регистрации
    bool login(char _login[LOGINLENGTH], char _pass[], int pass_length);            // метод авторизации
    void add(char _login[LOGINLENGTH], uint* pas_sha1_hash);                        // метод добавления пользователя
    void del(char _login[LOGINLENGTH]);                                             // метод удаления
    void resize();                                                                  // метод пересчета ХТ

    void printStats() const;
    void show();
private:
    enum enPairStatus   // перечисления
    {
        free,           // свободен
        engaged,        // занят
        deleted         // удален
    };

    struct AuthData 
    {
        AuthData(): login(""), pass_sha1_hash(0) {}                     // конструктор по умолчанию
        ~AuthData()                                                     // деструктор
        {
            if(pass_sha1_hash != 0 )
            {
                delete [] pass_sha1_hash;
            }
        }
        AuthData(char _login[LOGINLENGTH], uint* sh1)                   // конструктор копирования
        {
            strcpy(login, _login);
            pass_sha1_hash = sh1;
            status = enPairStatus::engaged;
        }
        AuthData& operator = (const AuthData& other)                    // конструктор присваивания
        {
            strcpy(login, other.login);
            if(pass_sha1_hash != 0)
            {
                delete [] pass_sha1_hash;
            }
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];
            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);
            return *this;
        }

        char login[LOGINLENGTH];    // ключ
        uint* pass_sha1_hash;       // значение
        enPairStatus status;        // статус ячейки
    };
    int hash_func(char _login[LOGINLENGTH], int offset);                      // хеш-функция
    AuthData* data;                                                     // массив с парой размером
    int data_count;                                                     // кол-во элементов
    int mem_size;                                                       // размер выделяемой памяти
};