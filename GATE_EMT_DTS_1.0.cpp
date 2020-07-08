// GATE_EMT_DTS_1.0.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "tcpgateW.h"

#define SIZEMASS 100

void f()
{
    next:
    Sleep(1000);
    goto next;
}

int main()
{
    /*std::thread t(f);
    HANDLE hand = t.native_handle();
    DWORD ress;
    std::cout << hand << std::endl;
    std::cout << GetThreadId(t.native_handle()) << std::endl;
next1:
    if (GetExitCodeThread(t.native_handle(), &ress))
    {
        std::cout << ress << std::endl;
    }
    else
    {
        std::cout << "ERROR " << GetLastError() <<std::endl;
    }
    Sleep(3000);
    goto next1;*/

    HANDLE mutex1;
    HANDLE memory1;
    char* bufmemory1;
    char* f;

    char ff=0;
    int tick = 0;
    HANDLE mutex2;
    HANDLE memory2;
    HANDLE semaphor2;
    char* bufmemory2;
    char* ibuf;

    mutex1= CreateMutexA(NULL, FALSE, "mutexsharmemory_emtdts_binar_in"); 
    memory1 = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SIZEMASS * 4, "sharmemory_emtdts_binar_in");
    bufmemory1 = (char*)MapViewOfFile(memory1, FILE_MAP_ALL_ACCESS, 0, 0, SIZEMASS * 4);

    mutex2 = CreateMutexA(NULL, FALSE, "mutexsharmemory_emtdts_binar_out");
    memory2 = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, SIZEMASS * 4, "sharmemory_emtdts_binar_out");
    bufmemory2 = (char*)MapViewOfFile(memory2, FILE_MAP_ALL_ACCESS, 0, 0, SIZEMASS * 4);
    semaphor2 = CreateSemaphoreA(NULL, 0, 1, "semaphorsharmemory_emtdts_binar_out");
 

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
    f = bufmemory1;
    std::cout << (int)*f << "  ";
    f = (bufmemory1+ SIZEMASS-1);
    std::cout << (int)*f << std::endl;
    ReleaseMutex(mutex1);

    ff = tick;
    if (ff < 0)
    {
        ff = 0;
        tick = 0;
    }
    WaitForSingleObject(mutex2, INFINITE);
    ibuf = bufmemory2;
    for (int i = 0; i < SIZEMASS; i++)
    {

        *ibuf= (char)ff%2;
        if (*ibuf == -1) std::cout <<"qweqweqweqweqwe -- " << (int)ff <<std::endl;
        ibuf++;
        ff++;
        if (ff < 0) ff = 0;
    }
    ReleaseMutex(mutex2);

    ReleaseSemaphore(semaphor2, 1, NULL);
    tick++;
    Sleep(1000);

    /*WaitForSingleObject(mutex1, INFINITE);
    f = (int*)bufmemory1;
    std::cout << *f << "  ";
    f = (int*)(bufmemory1+ SIZEMASS*4-4);
    std::cout << *f << std::endl;
    ReleaseMutex(mutex1);

    ff = tick;
    WaitForSingleObject(mutex2, INFINITE);
    ibuf = bufmemory2;
    for (int i = 0; i < SIZEMASS; i++)
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
    Sleep(1000);*/
    goto next;
}
