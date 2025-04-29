#include "chat.h"
#include <iostream>
#include <iomanip>

HashTable::HashTable() 
{
    data_count = 0;
    mem_size = 8;
    data = new AuthData[mem_size];
}

HashTable::~HashTable()
{
    delete[] data;
}

void HashTable::reg(char _login[LOGINLENGTH], char _pass[], int pass_length)
{
    add(_login, sha1(_pass, pass_length));
}

bool HashTable::login(char _login[LOGINLENGTH], char _pass[], int pass_length)
{   
    int i = 0;
    for(; i<data_count; i++)
    {
        AuthData& ad = data[i];
        if (!strcmp(ad.login, _login))
        {
            break;
        }
    }
    if(i >= data_count) return false;
    
    uint* digest = sha1(_pass, pass_length);
    
    bool cmpHashes = !memcmp(data[i].pass_sha1_hash, digest, SHA1HASHLENGTHBYTES);
    delete[] digest;
    return cmpHashes;

}

void HashTable::add(char _login[LOGINLENGTH], uint* pas_sha1_hash)
{
    for (int i = 0; i < mem_size; ++i)
    {
        if (data_count >= mem_size * 0.6)
        {
            std::cout << "Таблица заполнена, необходимо изменить размер..." << std::endl;
            resize();
            add(_login, pas_sha1_hash);
            return;
        }
        
        int index = hash_func(_login, i);
        // Если ячейка свободна или удалена
        if (data[index].status != enPairStatus::engaged)
        {
            // Освобождаем память старого хеша, если он был
            if (data[index].status == enPairStatus::deleted)
            {
                delete[] data[index].pass_sha1_hash;
            }
            // Копируем логин и хеш
            strcpy(data[index].login, _login);
            data[index].pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];
            memcpy(data[index].pass_sha1_hash, pas_sha1_hash, SHA1HASHLENGTHBYTES);
            data[index].status = enPairStatus::engaged;
            data_count++;
            std::cout << "Пользователь " << _login << " успешно зарегистрирован" << std::endl;
            return;
        }
    }
}

void HashTable::del(char _login[LOGINLENGTH])
{
    for (int i = 0; i < mem_size; ++i) 
    {
        int index = hash_func(_login, i);                          // находим индекс от ХФ
        if (data[index].status == enPairStatus::free)              // если ячейка свободна значи ключ никогда не добавлялся 
        {
            std::cout << "Ключ не найден - " << _login << std::endl;
            return;                                         // прерываем поиск и просто выходим
        }

        if (data[index].status == enPairStatus::engaged && strcmp(data[index].login, _login) == 0)      // если ячейка занята и имя совпадает с искомым - ключ найден
        {
            std::cout << "Ключ найден и удален - " << _login << " " << index << std::endl;
            data[index].status = enPairStatus::deleted;            // обозначаем как удаленный
            --data_count;                                        // уменьшаем счетчик
            return;                                         // выходим
        }
    }
    std::cout << "Ключ не найден после полного поиска - " << _login << std::endl;
}

void HashTable::resize()
{
    AuthData* save_ct = data;
    int old_size = mem_size;
    mem_size *= 2;
    data_count = 0;
    data = new AuthData[mem_size];
    std::cout << "Изменение хеш-таблицы с " << old_size << " в " << mem_size << std::endl;
    for (int i = 0; i < old_size; i++)
    {
        AuthData& ol_pair = save_ct[i];
        if (save_ct[i].status == enPairStatus::engaged)
        {
            // add(save_ct[i].fruit_name, save_ct[i].fruit_count);
            add(ol_pair.login, ol_pair.pass_sha1_hash);
        }
    }

    delete[] save_ct;
}

int HashTable::hash_func(char _login[LOGINLENGTH], int offset)
{
    int sum = 0, i = 0;
    for (; i < strlen(_login); i++)
    {
        sum += _login[i];
    }
    const double A = 0.6;
    int hf2 = mem_size * (A * sum - int(A * sum));
    // метод умножения - h(k) = [m * {k * a}] - пример int(mem_size * (val * a - int(a * val))) + квадратичные пробы(offset * ofset) % mem_size 
    return (hf2 + offset * offset) % mem_size;
}

void HashTable::printStats() const 
{
    std::cout << "Статус Хеш-Таблицы:" << std::endl;
    std::cout << "Размер: " << mem_size << std::endl;
    std::cout << "Кол-во Элементов: " << data_count << std::endl;
}

void HashTable::show()
{
    std::cout << "+-----------+-----------------+---------------+" << std::endl;
    std::cout << "|   Индекс  |         Ключ    |  Значение     |" << std::endl;
    std::cout << "+-----------+-----------------+---------------+" << std::endl;
    
    for (int i = 0; i < mem_size; i++)
    {
        std::cout << "| " << std::setw(9) << i << " | ";
        if(data[i].status == enPairStatus::free)
        {
            std::cout << std::setw(14) << "[FREE]" << " | " << std::setw(14) << " " << " |";
        }
        else if (data[i].status == enPairStatus::deleted)
        {
            std::cout << std::setw(14) << "[DELETED]" << " | " << std::setw(14) << " " << " |";
        }
        else
        {
            std::cout << std::setw(14) << data[i].login << " | " << std::setw(14) << data[i].pass_sha1_hash << " |";
        }
        std::cout << std::endl;
    }
}