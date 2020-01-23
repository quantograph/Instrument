#include "SdCard.h"

//=================================================================================================
SdCard::SdCard() {
}

//=================================================================================================
// Initializes the menu
void SdCard::Init() {
    Serial.println("SdCard::Init");
    if(!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("##### No SD card");
    }
}

//=================================================================================================
bool SdCard::ReadFile(const char* path, char*& data, uint32_t& dataSize) {
    File file;
    uint32_t bytesRead = 0;

    data = nullptr;
    dataSize = 0;

    // Open the file
    file = SD.open(path);
    if(!file) {
        Serial.printf("Can't open '%s' file\n", path);
        return false;
    }
    
    // Get the file size and read if its all bytes, one at a time
    dataSize = file.size();
    Serial.printf("Opened '%s' file, size=%d\n", path, dataSize);
    data = (char*)malloc(dataSize);
    if(data == NULL) {
        Serial.printf("Can't allocate %d bytes\n", dataSize);
    }

    while(file.available()) {
    	data[bytesRead++] = file.read();
    }

    if(bytesRead != dataSize)
        Serial.printf("Read %d bytes out of %d\n", bytesRead, dataSize);

    file.close();

    return true;
}

//=================================================================================================
void SdCard::PrintDirectory(File dir, int numTabs) {
    //Serial.println("SdCard::PrintDirectory");
    while(true) {
        File entry =  dir.openNextFile();
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
            PrintDirectory(entry, numTabs + 1);
        } else {
            // files have sizes, directories do not
            Serial.print("\t\t");
            Serial.printf("%d\n", entry.size(), DEC);
        }

        entry.close();
    }
}

//=================================================================================================
void SdCard::Test() {
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
    Serial.println("error1 opening test.txt");
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
    Serial.println("error2 opening test.txt");
  }

    PrintDirectory(SD.open("/"), 0);
}
