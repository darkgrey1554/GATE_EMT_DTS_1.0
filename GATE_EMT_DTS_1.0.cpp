// GATE_EMT_DTS_1.0.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "tcpgateW.h"

int main()
{

    HANDLE mutex1;
    HANDLE memory1;
    char* bufmemory1;
    float* f;

    float ff=0.;
    float tick = 0.;
    HANDLE mutex2;
    HANDLE memory2;
    HANDLE semaphor2;
    char* bufmemory2;
    char* ibuf;

    mutex1= CreateMutexA(NULL, FALSE, "mutexsharmemory_emtdts_analog_in"); 
    memory1 = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 100 * 4, "sharmemory_emtdts_analog_in");
    bufmemory1 = (char*)MapViewOfFile(memory1, FILE_MAP_ALL_ACCESS, 0, 0, 100 * 4);

    mutex2 = CreateMutexA(NULL, FALSE, "mutexsharmemory_emtdts_analog_out");
    memory2 = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 100 * 4, "sharmemory_emtdts_analog_out");
    bufmemory2 = (char*)MapViewOfFile(memory2, FILE_MAP_ALL_ACCESS, 0, 0, 100 * 4);
    semaphor2 = CreateSemaphoreA(NULL, 0, 1, "semaphorsharmemory_emtdts_analog_out");
 

    std::cout << "Hello World!\n";

    ConfigReader* config= new ConfigReader();
    config->ReadConfigFile("conf.txt");
    std::cout << config->NumberItem() << std::endl;
    std::list<TCPUnit*> tcpunit;

    for (int i = 0; i < config->NumberItem(); i++)
    {
        tcpunit.push_back(TCPUnit::create_tcp_unit(config->ReadConfigItem(i)));
        Sleep(10);
    }

next:



    WaitForSingleObject(mutex1, INFINITE);
    f = (float*)bufmemory1;
    std::cout << *f << "  ";
    f = (float*)(bufmemory1+396);
    std::cout << *f << std::endl;
    ReleaseMutex(mutex1);

    ff = tick;
    WaitForSingleObject(mutex2, INFINITE);
    ibuf = bufmemory2;
    for (int i = 0; i < 100; i++)
    {
        for (int i = 0; i < 4; i++)
        {
            *ibuf= *(((char*)&ff) + i);
            ibuf++;
        }
        ff++;
    }
    ReleaseMutex(mutex2);

    ReleaseSemaphore(semaphor2, 1, NULL);
    tick++;
    Sleep(100);
    goto next;
}
