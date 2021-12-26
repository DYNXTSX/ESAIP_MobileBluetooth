void serial_Write(char choice[]) {
	
	HANDLE hComm;    // using the serial port
    char ComPortName[] = "\\\\.\\COM3";    // it depends on device
    BOOL Status;

    hComm = CreateFile(ComPortName, 
    GENERIC_READ | GENERIC_WRITE, 
    0, 
    NULL, 
    OPEN_EXISTING, 
    0, 
    NULL);    // opening the serial port

    if (hComm == INVALID_HANDLE_VALUE)
    {
        fprintf(stderr, "Error\n");
    }
    else fprintf(stderr, "OK\n");

    /*
    DCB dcbSerialParams = {0};    // setting the parameters for the serial port
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);    // DCB = Data Control Block

    Status = GetCommState(hComm, &dcbSerialParams);    // retrieving the current settings

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    Status = SetCommState(hComm, &dcbSerialParams);    // configuring the port according to dcb structures

    COMMTIMEOUTS timeouts = {0};    // setting timeouts

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    char lpBuffer[10];    // determining the data to be written to the serial port

    strcpy(lpBuffer, choice);
    
	DWORD dNoOFBytestoWrite;
    DWORD dNoOfBytesWritten = 0;

    dNoOFBytestoWrite = sizeof(lpBuffer);    // calculating the amount of bytes

    Status = WriteFile(hComm, "ceciestuntest", dNoOFBytestoWrite, &dNoOfBytesWritten, NULL);    // writing the data to the serial port
                       
    CloseHandle(hComm);    // closing the serial port
	*/
}

