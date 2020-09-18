#include "Devices.h"
#include "SdCard.h"

//=================================================================================================
SdCard::SdCard() {
}

//=================================================================================================
// Initializes the menu
void SdCard::init() {
    //Serial.println("SdCard::Init");
    if(!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("##### No SD card");
    }
}

//=================================================================================================
bool SdCard::makeDir(const char* path) {
  return SD.mkdir(path);
}

//=================================================================================================
bool SdCard::writeFile(const char* path, const char* data, uint32_t dataSize) {
    File file;
    size_t written;

    // Open the file
    file = SD.open(path, FILE_WRITE);
    if(!file) {
        Serial.printf("##### ERROR: Can't open '%s' file for writing\n", path);
        return false;
    }

    // Write the data 
    file.seek(0);
    written = file.write(data, dataSize);
    if(written != dataSize)
        Serial.printf("##### ERROR: Wrote only %d out of %\n", written, dataSize);

	// close the file:
    file.close();

    return true;
}

//=================================================================================================
bool SdCard::readFile(const char* path, char*& data, uint32_t& dataSize) {
    File file;
    uint32_t bytesRead = 0;

    data = nullptr;
    dataSize = 0;

    // Open the file
    file = SD.open(path);
    if(!file) {
        Serial.printf("##### ERROR: Can't open '%s' file for reading\n", path);
        return false;
    }
    
    // Get the file size and read if its all bytes, one at a time
    dataSize = file.size();
    //Serial.printf("Opened '%s' file, size=%d\n", path, dataSize);
    data = (char*)malloc(dataSize);
    if(data == nullptr) {
        Serial.printf("##### ERROR: Can't allocate %d bytes\n", dataSize);
    }

    while(file.available()) {
    	data[bytesRead++] = file.read();
    }

    if(bytesRead != dataSize)
        Serial.printf("##### ERROR: Read %d bytes out of %d\n", bytesRead, dataSize);

    file.close();

    return true;
}

//=================================================================================================
void SdCard::printDirectory(File dir, int numTabs) {
    //Serial.println("SdCard::PrintDirectory");
    while(true) {
        File entry = dir.openNextFile();
        if(!entry) {
            // no more files
            //Serial.println("**nomorefiles**");
            break;
        }

        for(uint8_t i = 0; i < numTabs; i++) {
            Serial.print('\t');
        }

        Serial.println(entry.name());
        if(entry.isDirectory()) {
            Serial.print("/");
            printDirectory(entry, numTabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.printf("%d\n", entry.size(), DEC);
        }

        entry.close();
    }
}

//=================================================================================================
void SdCard::test() {
    File myFile;

    //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
    //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
    //SPI.setSCK(14);  // Audio shield has SCK on pin 14
  
    Serial.println("Initializing SD card...");

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    myFile = SD.open("test.txt", FILE_WRITE);
  
    // if the file opened okay, write to it:
    if (myFile) {
        Serial.println("Writing to test.txt...");
        myFile.println("testing 1, 2, 3.");
        // close the file:
        myFile.close();
        Serial.println("done.");
    } else {
        // if the file didn't open, print an error:
        Serial.println("##### ERROR: error1 opening test.txt");
    }
  
    // re-open the file for reading:
    myFile = SD.open("test.txt");
    if (myFile) {
        Serial.println("test.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
        Serial.write(myFile.read());
    }
        // close the file:
        myFile.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("##### ERROR: error2 opening test.txt");
    }

    //printDirectory(SD.open("/"), 0);
}
