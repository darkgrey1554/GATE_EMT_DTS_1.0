#pragma once
#include <iostream>
#include <list>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
#include <windows.h>
#include "logger.h"

#define TIME_DIV 20


enum class TypeData
{
	EMPTY,
	ANALOG,
	DISCRETE,
	BINAR,
	GROUP
};

enum class TypeSignal
{
	EMPTY,
	OUTPUT,
	INPUT
};

std::ostream& operator<<(std::ostream& out, TypeSignal& m);
std::ostream& operator<<(std::ostream& out, TypeData& m);


struct ConfigInfoTCPUnit
{
	TypeData type_data=TypeData::EMPTY;
	TypeSignal type_signal=TypeSignal::EMPTY;
	int size_data=0;
	std::string ip_address;
	std::string namesharedmemory;
	int port=0;
	int frequency_time=0;
	int id_unit=-1;

	void clear();
};


class ConfigReader
{
	std::list<ConfigInfoTCPUnit> listinfo;
	LoggerSpace::Logger* logger = LoggerSpace::Logger::getpointcontact();

public:

	ConfigReader();
	~ConfigReader();
	int ReadConfigFile(const char* namefile);
	ConfigInfoTCPUnit ReadConfigItem(int number);
	int NumberItem();
};

class TCPUnit
{
public:

	ConfigInfoTCPUnit set;

	void virtual restart_thread() {};
	void virtual close_tcp_unit() {};
	void virtual status_thread() {};

	static TCPUnit* create_tcp_unit(ConfigInfoTCPUnit unit);
};

class TCPServer: public TCPUnit
{
	
	std::thread thread_unit;

	int thread_tcp_server();

public:

	TCPServer(ConfigInfoTCPUnit confgate);
	void restart_thread() override;
	void close_tcp_unit() override;
	void status_thread() override;

};

class TCPClient: public TCPUnit
{
	std::thread thread_unit;

	int thread_tcp_client();

public:

	TCPClient(ConfigInfoTCPUnit confgate);
	void restart_thread() override;
	void close_tcp_unit() override;
	void status_thread() override;
};