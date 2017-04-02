#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <conio.h>
#include <process.h>
#include <locale.h>
#include <fstream>
#include <stdlib.h>
#include "Customer.h"
using namespace std;

#define BUFFSIZE 256

DWORD main(int argc, char *argv[])
{
	// Идентификатор канала Pipe
	HANDLE hNamedPipe;

	// Количество байт, переданных через канал
	DWORD  cbWritten;

	// Количество байт, принятых через канал
	DWORD  cbRead;

	// Буфер для передачи данных
	

	// Буфер для имени канала Pipe
	char   szPipeName[256];


	printf("Welcome to the MusicStore!\n");

	// Если при запуске PIPEC было указано имя срвера,
	// указываем его в имени канала Pipe
	if (argc > 1)
		sprintf_s(szPipeName, "\\\\%s\\pipe\\$MusicStorePipe",
			argv[1]);

	// Если имя сервера задано не было, создаем канал
	// с локальным процессом
	else
		strcpy_s(szPipeName, "\\\\.\\pipe\\$MusicStorePipe$");

	// Создаем канал с процессом PIPES
	hNamedPipe = CreateFile(
		szPipeName, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, 0, NULL);

	// Если возникла ошибка, выводим ее код и 
	// завершаем работу приложения
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		fprintf(stdout, "CreateFile: Error %ld\n",
			GetLastError());
		_getch();
		return 0;
	}

	string command, genre;
	Customer user(5000);

	// Выводим сообщение о создании канала
	fprintf(stdout, "\nConnected.Type your request or type 'exit' to terminate.\n");

	// Цикл обмена данными с серверным процессом
	while (1)
	{

		string   szBuf;
		// Выводим приглашение для ввода команды
		printf("cmd>");

		// Вводим текстовую строку
		getline(cin, szBuf);

		if (!strcmp(szBuf.c_str(), "buy")) {
			// Передаем введенную строку серверному процессу
			// в качестве команды
			if (!WriteFile(hNamedPipe, szBuf.c_str(), szBuf.length(),
				&cbWritten, NULL))
				break;
			command = "buy";
			szBuf.clear();

			char Buff[BUFFSIZE];
			DWORD cbRead = 512, i;
			memset(Buff, 0, sizeof(Buff) / sizeof(Buff[0]));
			// Получаем эту же команду обратно от сервера
			if (ReadFile(hNamedPipe, Buff, 512, &cbRead, NULL)) {

				//cout.ignore(numeric_limits < streamsize >::max(), '\n');
				//cout << "Received from MusicStore: %s\n" << szBuf.c_str();
				int i;
				for (i = 0; i < cbRead; i++)
				{
					cout << Buff[i];
				}
				printf("\n");
				memset(Buff, 0, sizeof(Buff) / sizeof(Buff[0]));
			}

			printf("cmd>");

			// Вводим текстовую строку
			getline(cin, szBuf);

			if (!strcmp(szBuf.c_str(), "rock") || !strcmp(szBuf.c_str(), "blues") || !strcmp(szBuf.c_str(), "jazz") ||
				!strcmp(szBuf.c_str(), "trip-hop")) {
				// Передаем введенную строку серверному процессу
				// в качестве команды
				if (!WriteFile(hNamedPipe, szBuf.c_str(), szBuf.length(),
					&cbWritten, NULL))
					break;
				genre = szBuf;
				szBuf.clear();

				char Buff[BUFFSIZE];
				DWORD cbRead = 512, i;
				memset(Buff, 0, sizeof(Buff) / sizeof(Buff[0]));
				// Получаем эту же команду обратно от сервера
				if (ReadFile(hNamedPipe, Buff, 512, &cbRead, NULL)) {

					//cout.ignore(numeric_limits < streamsize >::max(), '\n');
					//cout << "Received from MusicStore: %s\n" << szBuf.c_str();
					int i;
					for (i = 0; i < cbRead; i++)
					{
						cout << Buff[i];
					}
					printf("\n");
					memset(Buff, 0, sizeof(Buff) / sizeof(Buff[0]));
				}

				printf("cmd>");

				// Вводим текстовую строку
				getline(cin, szBuf);

				if ((command == "buy") && (!strcmp(genre.c_str(), "rock") || !strcmp(genre.c_str(), "blues") || !strcmp(genre.c_str(), "jazz") ||
					!strcmp(genre.c_str(), "trip-hop"))) {
					// Передаем введенную строку серверному процессу
					// в качестве команды
					if (!WriteFile(hNamedPipe, szBuf.c_str(), szBuf.length(),
						&cbWritten, NULL))
						break;
					genre = szBuf;
					szBuf.clear();

					char Buff[BUFFSIZE];
					DWORD cbRead = 512, i;
					memset(Buff, 0, sizeof(Buff) / sizeof(Buff[0]));
					// Получаем эту же команду обратно от сервера
					string price;
					if (ReadFile(hNamedPipe, Buff, 512, &cbRead, NULL)) {

						//cout.ignore(numeric_limits < streamsize >::max(), '\n');
						//cout << "Received from MusicStore: %s\n" << szBuf.c_str();
						int i;
						for (i = 0; i < cbRead; i++)
						{
							cout << Buff[i];
							price += Buff[i];
						}
						printf("\n");
						memset(Buff, 0, sizeof(Buff) / sizeof(Buff[0]));
						user.takeMyMoney(atoi(price.c_str()));
						user.getCash();
					}
				}
			}
		}
	


		// Если произошла ошибка, выводим ее код и
		// завершаем работу приложения
		else
		{
			fprintf(stdout, "ReadFile: Error %ld\n",
				GetLastError());
			_getch();
			break;
		}

		// В ответ на команду "exit" завершаем цикл
		// обмена данными с серверным процессом
		if (!strcmp(szBuf.c_str(), "exit"))
			break;
	}

	// Закрываем идентификатор канала
	CloseHandle(hNamedPipe);
	return 0;
}