#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdint.h>

int main(void)
{
	HANDLE hComm;                          // Handle to the Serial port
	char ComPortName[] = "\\\\.\\COM5"; // Name of the Serial port(May Change) to be opened,
	BOOL   Status;
	DWORD dwEventMask;                     // Event mask to trigger
	char  TempChar;                        // Temperory Character
	char  SerialBuffer[256];               // Buffer Containing Rxed Data
	DWORD NoBytesRead;                     // Bytes read by ReadFile()
	int i = 0;


	printf("\n\n +==========================================+");
	printf("\n |  Serial Communication (Win32 API)         |");
	printf("\n +==========================================+\n");
	/*----------------------------------- Opening the Serial Port --------------------------------------------*/

	hComm = CreateFile(ComPortName,                       // Name of the Port to be Opened
		GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
		0,                                 // No Sharing, ports cant be shared
		NULL,                              // No Security
		OPEN_EXISTING,                     // Open existing port only
		0,                                 // Non Overlapped I/O
		NULL);                             // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		printf("\n   Error! - Port %s can't be opened", ComPortName);
	else
		printf("\n   Port %s Opened\n ", ComPortName);


	/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/

	DCB dcbSerialParams = { 0 };                        // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);     //retreives  the current settings

	if (Status == FALSE)
		printf("\n   Error! in GetCommState()");

	dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
	dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	dcbSerialParams.Parity = EVENPARITY;      // Setting Parity = None 

	Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

	if (Status == FALSE)
	{
		printf("\n   Error! in Setting DCB Structure");
	}
	else
	{
		printf("\n   Setting DCB Structure Successfull\n");
		printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
		printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
		printf("\n       StopBits = %d", dcbSerialParams.StopBits);
		printf("\n       Parity   = %d", dcbSerialParams.Parity);
	}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/

	while (1)
	{

		COMMTIMEOUTS timeouts = { 0 };

		timeouts.ReadIntervalTimeout = 5000;
		timeouts.ReadTotalTimeoutConstant = 5000;
		timeouts.ReadTotalTimeoutMultiplier = 1000;
		timeouts.WriteTotalTimeoutConstant = 5000;
		timeouts.WriteTotalTimeoutMultiplier = 1000;

		if (SetCommTimeouts(hComm, &timeouts) == FALSE)
			printf("\n   Error! in Setting Time Outs");
		else
			printf("\n\n   Setting Serial Port Timeouts Successfull");

		printf("\n |---------  Serial Communication (Win32 API) --------|");

		printf("Starting to write......");

		char   lpBuffer[] = "ABC";		       // lpBuffer should be  char or byte array, otherwise write wil fail
		DWORD  dNoOFBytestoWrite;              // No of bytes to write into the port
		DWORD  dNoOfBytesWritten = 0;          // No of bytes written to the port

		dNoOFBytestoWrite = sizeof(lpBuffer); // Calculating the no of bytes to write into the port

		Status = WriteFile(hComm,               // Handle to the Serialport
			lpBuffer,            // Data to be written to the port 
			dNoOFBytestoWrite,   // No of bytes to write into the port
			&dNoOfBytesWritten,  // No of bytes written to the port
			NULL);

		if (Status == TRUE)
			printf("\n\n    %s - Written to %s", lpBuffer, ComPortName);
		else
			printf("\n\n   Error %d in Writing to Serial Port", GetLastError());


		/*-----------------------------------Finish Writing --------------------------------------------*/
		int k;
		for (k = 0; k < 100; k++)
		{
			printf("");
		}


		/*----------------------------------- Start Read--------------------------------------------*/
		dwEventMask = 1;

		//Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received

		/*-------------------------- Program will Wait here till a Character is received ------------------------*/

		if (Status == FALSE)
		{
			printf("\n    Error! in Setting WaitCommEvent()");
		}
		else //If  WaitCommEvent()==True Read the RXed data using ReadFile();
		{
			printf("\n\n    Characters Received");
			do
			{
				Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
				SerialBuffer[i] = TempChar;
				i++;
			} while (NoBytesRead > 0);

			/*------------Printing the RXed String to Console----------------------*/

			printf("\n\n    ");
			int j = 0;
			for (j = 0; j < i - 1; j++)		// j < i-1 to remove the dupliated last character
				printf("%c", SerialBuffer[j]);
		}
	}

	CloseHandle(hComm);//Closing the Serial Port
	printf("\n ==========================================\n");
	_getch();

}

