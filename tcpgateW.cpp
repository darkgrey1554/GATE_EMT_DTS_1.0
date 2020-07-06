#include "tcpgateW.h"

#define NUM_DATA_WRITE 1

std::ostream& operator<<(std::ostream& out, TypeSignal& m)
{
    if (m == TypeSignal::INPUT) out << "InPut";
    if (m == TypeSignal::OUTPUT) out << "OutPut";
    if (m == TypeSignal::EMPTY) out << "Empty";
    return out;
}

std::ostream& operator<<(std::ostream& out, TypeData& m)
{
    if (m == TypeData::ANALOG) out << "Analog";
    if (m == TypeData::DISCRETE) out << "Discrete";
    if (m == TypeData::BINAR) out << "Binar";
    if (m == TypeData::GROUP) out << "Group";
    if (m == TypeData::EMPTY) out << "Empty";
    return out;
}

void ConfigInfoTCPUnit::clear()
{
    type_data = TypeData::EMPTY;
    type_signal = TypeSignal::EMPTY;
    size_data = 0;
    ip_address.clear();
    namesharedmemory.clear();
    port=0;
    frequency_time = 0;
    id_unit = -1;
}

int ConfigReader::ReadConfigFile(const char* filename)
{

    FILE* config_file = NULL;
    char simvol = 0;
    std::string str_info;
    std::string helpstr;
    int res_read = 0;
    int pos[2] = { 0,0 };
    ConfigInfoTCPUnit unit;
    int count = 0;

    config_file = fopen(filename, "r");
    if (config_file == NULL)
    {
        std::cout << "MAIN\tERROR_OPEN_CONFIG_FILE" << std::endl;
        return -1;
    }

    for (;;)
    {
        simvol = 0;
        str_info.clear();
        while (simvol != '\n' && res_read != EOF)
        {
            res_read = fscanf(config_file, "%c", &simvol);
            if ((simvol > 0x1F || simvol == '\t') && res_read != EOF) str_info += simvol;
        }

        if (str_info == "[List]" || res_read == EOF)
        {
            if (res_read == EOF)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE1" << std::endl;
                return -1;
            }
            break;
        }

        if (str_info.substr(0, 4) == "@EMT")
        {
            pos[0] = 0;
            pos[1] = 0;
            unit.clear();

            if (str_info.find("Input") != -1)
            {
                unit.type_signal = TypeSignal::INPUT;
            }
            else if (str_info.find("Output") != -1)
            {
                unit.type_signal = TypeSignal::OUTPUT;
            }
            else
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE2" << std::endl;
                return -1;
            }

            if (str_info.find("Discrete") != -1)
            {
                unit.type_data = TypeData::DISCRETE;
            }
            else if (str_info.find("Analog") != -1)
            {
                unit.type_data = TypeData::ANALOG;
            }
            else if (str_info.find("Binar") != -1)
            {
                unit.type_data = TypeData::BINAR;
            }
            else if (str_info.find("Group") != -1)
            {
                unit.type_data = TypeData::GROUP;
            }
            else
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE3" << std::endl;
                return -1;
            }

            pos[0] = str_info.find('\t', 0);
            if (pos[0] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE4" << std::endl;
                return -1;
            }

            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE5" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.size_data = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE6" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.namesharedmemory = helpstr;

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE7" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.ip_address = helpstr;
            
            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE8" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.port = atoi(helpstr.c_str());

            pos[0] = pos[1];
            pos[1] = str_info.find('\t', (size_t)pos[0] + 1);
            if (pos[1] == -1)
            {
                std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE9" << std::endl;
                return -1;
            }
            helpstr.clear();
            helpstr = str_info.substr((size_t)pos[0] + 1, (size_t)pos[1] - pos[0] - 1);
            unit.frequency_time = atoi(helpstr.c_str());

            unit.id_unit = count;
            count++;
            listinfo.push_back(unit);

        } 
        else if (str_info.substr(0, 5) != "[EMT]")
        {
            std::cout << "MAIN\tERROR_FORMATION_OF_CONFIG_FILE10" << std::endl;
            return -1;
        }
    }
    fclose(config_file);
    return 0;
}

ConfigReader::ConfigReader()
{

}

ConfigReader::~ConfigReader()
{}

int ConfigReader::NumberItem()
{
    return listinfo.size();
}

ConfigInfoTCPUnit ConfigReader::ReadConfigItem(int number)
{
    if (number > listinfo.size())
    {
        ConfigInfoTCPUnit conf;
        conf.clear();
        return conf;
    }
    auto iter = listinfo.begin();
    for (int i = 0; i < number; i++)
    {
        iter++;
    }
    return (*iter);
}


TCPUnit* TCPUnit::create_tcp_unit(ConfigInfoTCPUnit gate)
{
    if (gate.type_signal == TypeSignal::OUTPUT) return new TCPServer(gate);
    if (gate.type_signal == TypeSignal::INPUT) return new TCPClient(gate);
}



TCPServer::TCPServer(ConfigInfoTCPUnit gate)
{
    set = gate;
    if (set.id_unit == -1 || set.ip_address.size() == 0 || set.namesharedmemory.size() == 0 || set.port == 0 ||
        set.type_data == TypeData::EMPTY || set.size_data == 0 || set.type_signal == TypeSignal::EMPTY)
    {
        std::cout << "SERVER ID:" << set.id_unit << "\tERROR_CONFIG_INFO" << std::endl;
    }
    else thread_unit = std::thread(&TCPServer::thread_tcp_server, this);
}

int TCPServer::thread_tcp_server()
{

    std::cout << "START SERVER ID:" << set.id_unit << "\tPORT:" << set.port << "\tIP_ADDRESS: "<<set.ip_address
        << "\tTYPEDATA:" << set.type_data << std::endl;

    int k_data = 0;
    if (set.type_data == TypeData::ANALOG || set.type_data == TypeData::DISCRETE) k_data = 4;
    if (set.type_data == TypeData::GROUP || set.type_data == TypeData::BINAR)     k_data = 1;

    DWORD last_error;

    std::string mutexname = "mutex";
    std::string semaphorname = "semaphor";
    HANDLE mutex = INVALID_HANDLE_VALUE;
    HANDLE semaphor = INVALID_HANDLE_VALUE;
    HANDLE memory = INVALID_HANDLE_VALUE;
    char* bufmemory;

    WSADATA wsadata;
    WSAOVERLAPPED send_overlapped;
    SecureZeroMemory((PVOID)&send_overlapped, sizeof(WSAOVERLAPPED));
    send_overlapped.hEvent = WSACreateEvent();

    SOCKET sock_server;
    SOCKADDR_IN addr_server;
    SOCKET connect_client;
    SOCKADDR_IN addr_client;
    int size_socket_client = sizeof(addr_client);   /// для accept
    DWORD result_wait_send;

    /// --- буферы для передачи данных --- ///

    char* ibuf;
    char* jbuf;
    char* buf_write = new char[set.size_data*k_data+4];
    DWORD flag_send = 0;
    DWORD count_send = 0;
    DWORD count_get_byte_send = 0;
    
    WSABUF wsabuf_write;
    wsabuf_write.buf = buf_write;
    wsabuf_write.len = set.size_data * k_data + 4;

    float* f;
    /// инициализация общей памяти 
    

    mutexname += set.namesharedmemory.c_str();
    semaphorname += set.namesharedmemory.c_str();

    mutex = CreateMutexA(NULL, FALSE, mutexname.c_str());
    if (mutex == NULL)
    {
        std::cout << "SERVER ID:"<<set.id_unit<<"\tERROR_INITIALIZATION_MUTEX\tCODE_ERROR: "<<GetLastError()<< std::endl;
        return -1;
    }
    semaphor = CreateSemaphoreA(NULL, 0, 1, semaphorname.c_str());
    if (semaphor == NULL)
    {
        std::cout << "SERVER ID:" << set.id_unit << "\tERROR_INITIALIZATION_SEMAPHOR\tCODE_ERROR: " << GetLastError() << std::endl;
        return -1;
    }

    memory = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, set.size_data*k_data, set.namesharedmemory.c_str());
    if (memory == NULL)
    {
        std::cout << "SERVER ID:" << set.id_unit << "\tERROR_INITIALIZATION_SHARED_MEMORY\tCODE_ERROR: " << GetLastError() << std::endl;
        return -1;
    }
    bufmemory = (char*)MapViewOfFile(memory, FILE_MAP_ALL_ACCESS, 0, 0, set.size_data * k_data);
    if (bufmemory == NULL)
    {
        std::cout << "SERVER ID:" << set.id_unit << "\tERROR_INITIALIZATION_MAPVIEW\tCODE_ERROR: " << GetLastError() << std::endl;
        return -1;
    }

    /// инициализация асинхранного сервера
    
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        std::cout << "SERVER ID:" << set.id_unit << "\tERROR_WSAStartup CODE_ERROR: " << WSAGetLastError() << std::endl;
        return -1;
    }

    sock_server = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (sock_server == INVALID_SOCKET)
    {
        last_error = WSAGetLastError();
        std::cout << "SERVER ID: " << set.id_unit << "\tERROR_INITIALIZATION_SOCKET CODE_ERROR: " << last_error << std::endl;
        Sleep(1000);
        return -1;
    }

    addr_server.sin_addr.s_addr = inet_addr(set.ip_address.c_str());
    addr_server.sin_port = htons(set.port);
    addr_server.sin_family = AF_INET;

    if (bind(sock_server, (sockaddr*)&addr_server, sizeof(addr_server)) == SOCKET_ERROR)
    {
        last_error = WSAGetLastError();
        std::cout << "SERVER ID: " << set.id_unit << "\tERROR_BIND_SOCKET CODE_ERROR: " << last_error << std::endl;
        closesocket(sock_server);
        Sleep(1000);
        return -1;
    }

    listen(sock_server, 1);

    for (;;) 
    {
        connect_client = accept(sock_server, (sockaddr*)&addr_client, &size_socket_client);
        if (connect_client == INVALID_SOCKET)
        {
            last_error = WSAGetLastError();
            std::cout << "SERVER ID: " << set.id_unit << "\tERROR_CONNECT_WITH_CLIENT CODE_ERROR: " << last_error << std::endl;
            Sleep(2000);
            continue;
        }
        else
        {
            std::cout << "SERVER ID: " << set.id_unit << "\tCONNECT_WITH_CLIENT_IP: "
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b1 << "."
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b2 << "."
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b3 << "."
                << (int)addr_client.sin_addr.S_un.S_un_b.s_b4
                << "  PORT: " << addr_client.sin_port << std::endl;

        }

        for (;;)
        {
            WaitForSingleObject(semaphor, INFINITE);

            ibuf = bufmemory;
            jbuf = buf_write;
            for (int i = 0; i < 4; i++)
            {
                *jbuf= *(((char*)&set.size_data) + i);
                jbuf++;
            }
            WaitForSingleObject(mutex, INFINITE);
            for (int i = 0; i < set.size_data * k_data; i++)
            {
                *jbuf= *ibuf;
                jbuf++;
                ibuf++;
            }
            ReleaseMutex(mutex);

            count_get_byte_send = 0;
            count_send = 0;

            for (;;)
            {
                flag_send = 0;
                wsabuf_write.buf = buf_write+count_send;
                wsabuf_write.len = set.size_data * k_data + 4-count_send;

                if (WSASend(connect_client, &wsabuf_write, 1, &count_get_byte_send, flag_send, &send_overlapped, NULL) == SOCKET_ERROR)
                {
                    last_error = WSAGetLastError();
                    if (last_error != WSA_IO_PENDING)
                    {
                        std::cout << "SERVER ID: " << set.id_unit << "\tERROR_SEND CODE_ERROR: " << last_error << std::endl;                
                        closesocket(connect_client);
                        Sleep(2000);
                        connect_client = INVALID_SOCKET;
                        break;                        
                    }

                }
                result_wait_send = WSAWaitForMultipleEvents(1, &send_overlapped.hEvent, TRUE, INFINITE, TRUE);
                if (result_wait_send == WSA_WAIT_FAILED)
                {
                    last_error = WSAGetLastError();
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR_WSASEND  CODE_ERROR: " << last_error << std::endl;
                    closesocket(connect_client);
                    Sleep(2000);
                    connect_client = INVALID_SOCKET;
                    break;
                }
                if (!WSAGetOverlappedResult(connect_client, &send_overlapped, &count_get_byte_send, FALSE, &flag_send))
                {
                    last_error = WSAGetLastError();
                    std::cout << "SERVER ID: " << set.id_unit << "\tERROR_WSASEND  CODE_ERROR: " << last_error << std::endl;
                    closesocket(connect_client);
                    Sleep(2000);
                    connect_client = INVALID_SOCKET;
                    break;
                }
                WSAResetEvent(send_overlapped.hEvent);

                count_send += count_get_byte_send;
                if (count_send < set.size_data * k_data + 4)
                {
                    continue;
                }
                else break;
            }
            if (connect_client == INVALID_SOCKET) break;            
        }
    }
    
    
    return 0;
}

void TCPServer::close_tcp_unit()
{
}

void TCPServer::restart_thread()
{
}

void TCPServer::status_thread()
{
}




TCPClient::TCPClient(ConfigInfoTCPUnit gate)
{
    set = gate;
    if (set.id_unit == -1 || set.ip_address.size() == 0 || set.namesharedmemory.size() == 0 || set.port == 0 ||
        set.type_data == TypeData::EMPTY || set.size_data == 0 || set.type_signal == TypeSignal::EMPTY)
    {
        std::cout << "CLIENT ID:" << set.id_unit << "\tERROR_CONFIG_INFO" << std::endl;
    }
    else thread_unit = std::thread(&TCPClient::thread_tcp_client, this);
}

int TCPClient::thread_tcp_client()
{
    std::cout << "START CLIENT ID:" << set.id_unit << "\tPORT:" << set.port << "\tIP_ADDRESS: " << set.ip_address
        << "\tTYPEDATA:" << set.type_data << std::endl;

    int k_data = 0;
    if (set.type_data == TypeData::ANALOG || set.type_data == TypeData::DISCRETE) { k_data = 4; }
    else if (set.type_data == TypeData::GROUP || set.type_data == TypeData::BINAR) k_data = 1;

    DWORD last_error;

    std::string mutexname = "mutex";
    std::string semaphorname = "semofor";
    HANDLE mutex = INVALID_HANDLE_VALUE;
    HANDLE semaphor = INVALID_HANDLE_VALUE;
    HANDLE memory = INVALID_HANDLE_VALUE;
    char* bufmemory;

    WSADATA wsadata;

    WSAOVERLAPPED recv_overlapped;
    SecureZeroMemory((PVOID)&recv_overlapped, sizeof(WSAOVERLAPPED));
    recv_overlapped.hEvent = WSACreateEvent();
   
    WSAOVERLAPPED send_overlapped;
    SecureZeroMemory((PVOID)&send_overlapped, sizeof(WSAOVERLAPPED));
    send_overlapped.hEvent = WSACreateEvent();

    SOCKET sock_client;
    SOCKADDR_IN addr_server;
    int size_socket_client = sizeof(addr_server);   /// для accept
    DWORD result_wait_recv;

    /// --- буферы для передачи данных --- ///
    
    char* ibuf;
    char* jbuf;
    char* buf_read = new char[set.size_data * k_data + 5];
    char* buf_write = new char[NUM_DATA_WRITE];
    
    DWORD flag_read = 0;
    DWORD count_read = 0;
    DWORD count_get_byte = 0;
    int num_data_from_server=0;
    int command = 0;

    DWORD count_send;
    DWORD flag_send = 0;
    DWORD result_wait_send = 0;

    WSABUF wsabuf_write;
    wsabuf_write.buf = buf_write;
    wsabuf_write.len = NUM_DATA_WRITE;

    WSABUF wsabuf_read;
    wsabuf_read.buf = buf_read;
    wsabuf_read.len = set.size_data * k_data + 5;   

    LARGE_INTEGER timenow;
    LARGE_INTEGER timelast;
    LARGE_INTEGER fhz;
    float time;
    QueryPerformanceFrequency(&fhz);

    /// инициализация общей памяти 


    mutexname += set.namesharedmemory.c_str();
    semaphorname += set.namesharedmemory.c_str();

    mutex = CreateMutexA(NULL, FALSE, mutexname.c_str());
    if (mutex == NULL)
    {
        std::cout << "CLIENT ID:" << set.id_unit << "\tERROR_INITIALIZATION_MUTEX\tCODE_ERROR: " << GetLastError() << std::endl;
        return -1;
    }
    semaphor = CreateSemaphoreA(NULL, 0, 1, semaphorname.c_str());
    if (semaphor == NULL)
    {
        std::cout << "CLIENT ID:" << set.id_unit << "\tERROR_INITIALIZATION_SEMAPHOR\tCODE_ERROR: " << GetLastError() << std::endl;
        return -1;
    }

    memory = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, set.size_data * k_data, set.namesharedmemory.c_str());
    if (memory == NULL)
    {
        std::cout << "CLIENT ID:" << set.id_unit << "\tERROR_INITIALIZATION_SHARED_MEMORY\tCODE_ERROR: " << GetLastError() << std::endl;
        return -1;
    }
    bufmemory = (char*)MapViewOfFile(memory, FILE_MAP_ALL_ACCESS, 0, 0, set.size_data * k_data);
    if (bufmemory == NULL)
    {
        std::cout << "CLIENT ID:" << set.id_unit << "\tERROR_INITIALIZATION_MAPVIEW\tCODE_ERROR: " << GetLastError() << std::endl;
        return -1;
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        std::cout << "CLIENT ID:" << set.id_unit << "\tERROR_WSAStartup CODE_ERROR: " << WSAGetLastError() << std::endl;
        return -1;
    }

    for (;;)
    {
        sock_client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (sock_client == INVALID_SOCKET)
        {
            last_error = WSAGetLastError();
            std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_INVALID_SOCKET CODE_ERROR: " << last_error << std::endl;
            Sleep(2000);
            return -1;
        }

        addr_server.sin_addr.s_addr = inet_addr(set.ip_address.c_str());
        addr_server.sin_port = htons(set.port);
        addr_server.sin_family = AF_INET;

        if (connect(sock_client, (sockaddr*)&addr_server, sizeof(addr_server)) == SOCKET_ERROR)
        {
            last_error = WSAGetLastError();
            std::cout << "CLIENT ID: "<< set.id_unit << "\tERROR_CONNECT_WITH_SERVER  CODE_ERROR: " << last_error << std::endl;
            closesocket(sock_client);
            Sleep(5000);
            continue;
        }
        else
        {
            std::cout << "CLIENT ID: " << set.id_unit << "\tCONNECT_WITH_SERVER_DONE " << std::endl;
        }
        count_get_byte = 0;
        count_read = 0;
        QueryPerformanceCounter(&timelast);
        QueryPerformanceCounter(&timenow);

        for (;;)
        {
            for (;;)
            {
                QueryPerformanceCounter(&timenow);
                time = (timenow.QuadPart - timelast.QuadPart) * 1000.0 / fhz.QuadPart;
                if (time >= set.frequency_time)
                {
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR TIME SINCE LAST REQUEST: "<< time << std::endl;
                    break;
                }
                if (time > set.frequency_time - TIME_DIV) break;
                Sleep(1);
            }
            QueryPerformanceCounter(&timelast);

            *buf_write = 3;
            count_send = 0;

            for (;;)
            {
                wsabuf_write.buf = buf_write+count_send;
                wsabuf_write.len = NUM_DATA_WRITE-count_send;

                if (WSASend(sock_client, &wsabuf_write, 1, &count_get_byte, flag_send, &send_overlapped, NULL) == SOCKET_ERROR)
                {
                    last_error = WSAGetLastError();
                    if (last_error != WSA_IO_PENDING)
                    {
                        std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_SEND_MESSENG  CODE_ERROR: " << last_error << std::endl;
                        closesocket(sock_client);
                        Sleep(2000);
                        sock_client = INVALID_SOCKET;
                        break;
                    }
                }

                result_wait_send = WSAWaitForMultipleEvents(1, &send_overlapped.hEvent, TRUE, INFINITE, TRUE);
                if (result_wait_send == WSA_WAIT_FAILED)
                {
                    last_error = WSAGetLastError();
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_WAIT_EVENT CODE_ERROR: " << last_error << std::endl;
                    closesocket(sock_client);
                    Sleep(2000);
                    sock_client = INVALID_SOCKET;
                    break;
                }

                WSAResetEvent(send_overlapped.hEvent);

                if (!WSAGetOverlappedResult(sock_client, &send_overlapped, &count_get_byte, FALSE, &flag_send))
                {
                    last_error = WSAGetLastError();
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_SEND_MESSENG CODE_ERROR: " << last_error << std::endl;
                    closesocket(sock_client);
                    Sleep(2000);
                    sock_client = INVALID_SOCKET;
                    break;
                }

                count_send += count_get_byte;

                if (count_send == 0)
                {
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_REQEST BAD SIZE DATA " << std::endl;
                    closesocket(sock_client);
                    Sleep(2000);
                    sock_client = INVALID_SOCKET;
                    break;
                }
                else if (count_send < NUM_DATA_WRITE) continue;
            }                 

            if (sock_client == INVALID_SOCKET)
            {
                break;
            }        
            
            count_read = 0;
            num_data_from_server = 0;

            for (;;)
            {
                wsabuf_read.buf = buf_read+count_read;
                wsabuf_read.len = set.size_data * k_data + 5-count_read;

                if (WSARecv(sock_client, &wsabuf_read, 1, &count_get_byte, &flag_read, &recv_overlapped, NULL) == SOCKET_ERROR)
                {
                    
                    last_error = WSAGetLastError();
                    if (last_error != WSA_IO_PENDING)
                    {
                        std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_RECV CLIENT_ID: CODE_ERROR: " << last_error << std::endl;
                        closesocket(sock_client);
                        Sleep(2000);
                        sock_client = INVALID_SOCKET;
                        break;
                    }
                }

                result_wait_recv = WSAWaitForMultipleEvents(1, &recv_overlapped.hEvent, TRUE, INFINITE, TRUE);
                if (result_wait_recv == WSA_WAIT_FAILED)
                {
                    last_error = WSAGetLastError();
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_WSA_WAIT_EVENT CODE_ERROR: " << last_error << std::endl;
                    closesocket(sock_client);
                    Sleep(2000);
                    sock_client = INVALID_SOCKET;
                    break;
                }

                WSAResetEvent(recv_overlapped.hEvent);

                if (!WSAGetOverlappedResult(sock_client, &recv_overlapped, &count_get_byte, FALSE, &flag_read))
                {
                    last_error = WSAGetLastError();
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_READ_SERVER CODE_ERROR: " << last_error << std::endl;
                    closesocket(sock_client);
                    Sleep(2000);
                    sock_client = INVALID_SOCKET;
                    break;
                }

                
                count_read += count_get_byte;
              
                if (count_read >= 5 && num_data_from_server == 0)
                {                    
                    command = *buf_read;
                    num_data_from_server = *((int*)(buf_read + 1));
                }

                if (num_data_from_server != set.size_data)
                {
                    std::cout << "CLIENT ID: " << set.id_unit << "\tERROR_RECEPTION BAD SIZE DATA (watch config file) " << std::endl;
                    closesocket(sock_client);
                    Sleep(2000);
                    sock_client = INVALID_SOCKET;
                    break;
                }

                if (count_read < num_data_from_server*k_data + 5)
                {
                    continue;
                }
                else
                {
                    break;
                }
            }

            if (sock_client == INVALID_SOCKET)
            {
                break;
            }           

            ibuf = bufmemory;
            jbuf = buf_read;
            jbuf += 5;
            WaitForSingleObject(mutex, INFINITE);
            for (int i = 0; i < set.size_data * k_data; i++)
            {
                *ibuf = *jbuf;
                jbuf++;
                ibuf++;
            }
            ReleaseMutex(mutex);

        }
    }
    return 0;
}

void TCPClient::close_tcp_unit()
{
}

void TCPClient::restart_thread()
{
}

void TCPClient::status_thread()
{
}

