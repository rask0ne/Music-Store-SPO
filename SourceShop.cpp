//#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "Shop.h"
using namespace std;
#define BUFFSIZE 256



int main()
{


	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));	// обнулить
	si.cb = sizeof(STARTUPINFO);			// указать размер

	if (CreateProcess("C:\\Users\\rask\\Documents\\Visual Studio 2015\\Projects\\Lab1SPO_Client\\Debug\\Lab1SPO_Client.exe",
		NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi) == TRUE)
	{
		//////////////////////////////
		// Флаг успешного создания канала
		BOOL   fConnected;
		// Идентификатор канала Pipe
		HANDLE hNamedPipe;
		// Имя создаваемого канала Pipe
		LPSTR  lpszPipeName = "\\\\.\\pipe\\$MusicStorePipe$";
		// Буфер для передачи данных через канал

		char Buff[BUFFSIZE];
		DWORD iNumBytesToRead = 512, i;
		
		// Количество байт данных, принятых через канал
		DWORD  cbRead;
		// Количество байт данных, переданных через канал
		DWORD  cbWritten;



		//cout << "Welcom to the MusicStore!\n";
		hNamedPipe = CreateNamedPipe(
			lpszPipeName,
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			512, 512, 5000, NULL);
		if (hNamedPipe == INVALID_HANDLE_VALUE)
		{
			fprintf(stdout, "CreateNamedPipe: Error %ld\n",
				GetLastError());
			_getch();
			return 0;
		}
		// Выводим сообщение о начале процесса создания канала
		fprintf(stdout, "Waiting for connect...\n");

		// Ожидаем соединения со стороны клиента
		fConnected = ConnectNamedPipe(hNamedPipe, NULL);
		// При возникновении ошибки выводим ее код
		if (!fConnected)
		{
			switch (GetLastError())
			{
			case ERROR_NO_DATA:
				fprintf(stdout, "ConnectNamedPipe: ERROR_NO_DATA");
				_getch();
				CloseHandle(hNamedPipe);
				return 0;
				break;

			case ERROR_PIPE_CONNECTED:
				fprintf(stdout,
					"ConnectNamedPipe: ERROR_PIPE_CONNECTED");
				_getch();
				CloseHandle(hNamedPipe);
				return 0;
				break;

			case ERROR_PIPE_LISTENING:
				fprintf(stdout,
					"ConnectNamedPipe: ERROR_PIPE_LISTENING");
				_getch();
				CloseHandle(hNamedPipe);
				return 0;
				break;

			case ERROR_CALL_NOT_IMPLEMENTED:
				fprintf(stdout,
					"ConnectNamedPipe: ERROR_CALL_NOT_IMPLEMENTED");
				_getch();
				CloseHandle(hNamedPipe);
				return 0;
				break;

			default:
				fprintf(stdout, "ConnectNamedPipe: Error %ld\n",
					GetLastError());
				_getch();
				CloseHandle(hNamedPipe);
				return 0;
				break;

			}

			CloseHandle(hNamedPipe);
			_getch();
			return 0;
		}

		// Выводим сообщение об успешном создании канала
		fprintf(stdout, "\nConnected. Waiting for customers request...\n");

		Shop shop(10, 15, 20, 12);
		
		// Цикл получения команд через канал
		while (1)
		{
			// Получаем очередную команду через канал Pipe
			if (ReadFile(hNamedPipe, Buff, 512, &cbRead, NULL))
			{
				string   szBuf(Buff, cbRead);
				cout << "Recieved from customer: " << szBuf.c_str();

				if (!strcmp(szBuf.c_str(), "buy")) {

					szBuf = shop.showGenres();
					if (!WriteFile(hNamedPipe, szBuf.c_str(), szBuf.length(), &cbWritten, NULL))
						break;

					szBuf.clear();

					if (ReadFile(hNamedPipe, Buff, 512, &cbRead, NULL))
					{
						string   szBuf(Buff, cbRead);
						string temp;
						if (!strcmp(szBuf.c_str(), "rock") || !strcmp(szBuf.c_str(), "blues") || !strcmp(szBuf.c_str(), "jazz") ||
							!strcmp(szBuf.c_str(), "trip-hop")) {

							temp = szBuf;

							szBuf = shop.showAlbums(szBuf);
							if (!WriteFile(hNamedPipe, szBuf.c_str(), szBuf.length(), &cbWritten, NULL))
								break;
						}

						szBuf.clear();

						if (ReadFile(hNamedPipe, Buff, 512, &cbRead, NULL))
						{
							string   szBuf(Buff, cbRead);
							int num = atoi(szBuf.c_str());
							if (num > 0)
								szBuf = shop.minusPrice(temp, num);
							if (!WriteFile(hNamedPipe, szBuf.c_str(), szBuf.length(), &cbWritten, NULL))
								break;
						}
					}
				}
			}
				else {
					string   szBuf(Buff, cbRead);
					cout << "\ncmd>";
					//cin >> szBuf;
					szBuf = "wrong message";
					if (!WriteFile(hNamedPipe, szBuf.c_str(), szBuf.length(), &cbWritten, NULL))
						break;

				}


				// Посылаем эту команду обратно клиентскому
				// приложению


				/*
				// Выводим принятую команду на консоль
				printf("Received: <%s>\n", szBuf);

				// Если пришла команда "exit",
				// завершаем работу приложения
				if (!strcmp(szBuf, "exit"))
				break;
				*/
			
			/*else
			{
				fprintf(stdout, "ReadFile: Error %ld\n",
					GetLastError());
				_getch();
				break;
			}*/
		}
		_getch();
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

