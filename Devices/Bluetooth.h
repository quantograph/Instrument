#ifndef Bluetooth_h
#define Bluetooth_h

class Bluetooth {
public:
    Bluetooth();
    bool Init();
    bool Send(const char* buffer, int size);
    bool Receive();
    bool Process();
    #define BUFFER_SIZE 1024
    char _buffer[BUFFER_SIZE];
    int _packetSize = 0;
    int _received = 0; // Received
    char _string[64];
};

#endif // #ifndef Bluetooth_h
