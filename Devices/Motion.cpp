//// Motion.ino - all functions related motion detection - acceleration and rotation
//
////=================================================================================================
//// Initializes the MPU6050 chip
//bool InitMotion() {
//    Wire.begin();
//    _mpu.initialize(); // All scales are set here
//
//    if(!_mpu.testConnection()) {
//        //Serial.println("##### MPU is not connected");
//        return false;
//    }
//
//    return true;
//}
//
////=================================================================================================
//// Reads the motion values
//void ReadMotion() {
//    //int noise = 30;
//    size_t sent; // Bytes sent
//
//    _motionRead = true;
//    _mpu.getMotion6(&_motionData._accelX, &_motionData._accelY, &_motionData._accelZ,
//                    &_motionData._rotX, &_motionData._rotY, &_motionData._rotZ); // Acceleration, gyroscope
//
//    /*_motionData._accelX = 1;
//    _motionData._accelY = 2;
//    _motionData._accelZ = 3;
//    _motionData._rotX = 4;
//    _motionData._rotY = 5;
//    _motionData._rotZ = 6;*/
//
//
//    //sprintf(_string, "%6d, %6d, %6d, %6d, %6d, %6d", ax, ay, az, gx, gy, gz);
//
//    /*_xPos += _ax;
//    _yPos += _ay;
//    _zPos += _az;*/
//
//    // Get the gyro changes
//    /*if(_gxPrev != MAX_INT16)
//        _gxChange = abs(_gxPrev - _gx);
//
//    if(_gxPrev != MAX_INT16)
//        _gyChange = abs(_gxPrev - _gx);
//
//    if(_gxPrev != MAX_INT16)
//        _gzChange = abs(_gxPrev - _gx);*/
//
//    // Add the angles
//    /*if(_gxChange > noise)
//        _xAngle += _gx;
//
//    if(_gyChange > noise)
//        _yAngle += _gy;
//
//    if(_gzChange > noise)
//        _zAngle += _gz;
//
//    ShowMotion();*/
//
//    // Get the drift
//    /*if(_gxPrev != MAX_INT16)
//        _gxTotal += abs(_gxPrev - _gx);
//
//    if(_gyPrev != MAX_INT16)
//        _gyTotal += abs(_gyPrev - _gy);
//
//    if(_gzPrev != MAX_INT16)
//        _gzTotal += abs(_gzPrev - _gz);
//
//    _motionCount++;
//    if(_motionCount % 10 == 0 && _motionCount > 0) {
//        _xAngle = _gxTotal / _motionCount;
//        _yAngle = _gyTotal / _motionCount;
//        _zAngle = _gzTotal / _motionCount;
//        //ShowMotion();
//
//        //sprintf(_string, "Drift: %6d, %6d, %6d", _gxTotal / _motionCount, _gyTotal / _motionCount, _gzTotal / _motionCount);
//        //Serial.println(_string);
//
//        _gxTotal = 0;
//        _gyTotal = 0;
//        _gzTotal = 0;
//        _motionCount = 0;
//    }
//
//    //delay(100);
//
//    _gxPrev = _gx;
//    _gyPrev = _gy;
//    _gzPrev = _gz;*/
//}
//
////=================================================================================================
//// Send motion readings to the computer
//void SendMotion() {
//    size_t sent; // Bytes sent
//
//    _motionData._sequence = _sequence++;
//    sent = Serial.write((const uint8_t*)&_motionData, _motionDataSize);
//    if(sent != _motionDataSize) {
//        sprintf(_string, "Returned %d", sent);
//        ShowError("Serial write", _string);
//    }
//}
//
////=================================================================================================
//// Shows the motion values
//void ShowMotion() {
//    /*_tft.setTextSize(1);
//    int32_t maxGyro = 0x7fff;
//    int32_t maxAccel = 0x7fff;
//    int32_t maxAccel2 = 0x1fff;
//    float value;
//    int y;
//
//    // Acceleration
//    _tft.setCursor(0, 0);
//    sprintf(_string, "%6d", _ax);
//    _tft.println(_string);
//    sprintf(_string, "%6d", _ay);
//    _tft.println(_string);
//    sprintf(_string, "%6d", _az);
//    _tft.println(_string);
//
//    y = 30;
//    ShowMotionLine((float)_ax / maxAccel, y, RED); y += 2;
//    ShowMotionLine((float)_ay / maxAccel, y, GREEN); y += 2;
//    ShowMotionLine((float)_az / maxAccel, y, BLUE); y += 2;
//
//    y = 40;
//    ShowMotionLine((float)_ax / maxAccel2, y, RED); y += 2;
//    ShowMotionLine((float)_ay / maxAccel2, y, GREEN); y += 2;
//    ShowMotionLine((float)_az / maxAccel2, y, BLUE); y += 2;*/
//
//    // Rotation
//    /*_tft.setCursor(0, 50);
//    sprintf(_string, "%12ld", _xAngle);
//    _tft.println(_string);
//    sprintf(_string, "%12ld", _yAngle);
//    _tft.println(_string);
//    sprintf(_string, "%12ld", _zAngle);
//    _tft.println(_string);*/
//
//    /*y = 90;
//    ShowMotionLine((double)_xAngle / maxGyro, y, RED); y += 2;
//    ShowMotionLine((double)_yAngle / maxGyro, y, GREEN); y += 2;
//    ShowMotionLine((double)_zAngle / maxGyro, y, BLUE); y += 2;*/
//}
//
////=================================================================================================
//// Shows a motion value as a colored line
//void ShowMotionLine(float value, int y, int16_t color) {
//    int width = _tft.width();
//    int middle = width / 2;
//    int length;
//
//    length = middle * value;
//    _tft.drawFastHLine(0, y, width, BLACK); // Erase the old line
//    if(length > 0)
//        _tft.drawFastHLine(middle, y, length, color);
//    else
//        _tft.drawFastHLine(middle + length, y, -length, color);
//}
//
////=================================================================================================
//// Gets min and max of all values
//// Min: -20172, -32768, -32768, -24588, -32768, -29598
//// Max:  32767,  30668,  32767,  25219,  32564,  18892
//void GetMotionMinMax() {
//
//    /*_mpu.getMotion6(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);
//    //sprintf(_string, "%6d, %6d, %6d, %6d, %6d, %6d", ax, ay, az, gx, gy, gz);
//
//    _max_ax = max(_max_ax, _ax);
//    _max_ay = max(_max_ay, _ay);
//    _max_az = max(_max_az, _az);
//    _min_ax = min(_min_ax, _ax);
//    _min_ay = min(_min_ay, _ay);
//    _min_az = min(_min_az, _az);
//
//    _max_gx = max(_max_gx, _gx);
//    _max_gy = max(_max_gy, _gy);
//    _max_gz = max(_max_gz, _gz);
//    _min_gx = min(_min_gx, _gx);
//    _min_gy = min(_min_gy, _gy);
//    _min_gz = min(_min_gz, _gz);
//
//    sprintf(_string, "Min: %6d, %6d, %6d, %6d, %6d, %6d", _min_ax, _min_ay, _min_az, _min_gx, _min_gy, _min_gz);
//    Serial.println(_string);
//    sprintf(_string, "Max: %6d, %6d, %6d, %6d, %6d, %6d", _max_ax, _max_ay, _max_az, _max_gx, _max_gy, _max_gz);
//    Serial.println(_string);
//    Serial.println(" ");
//
//    delay(100);*/
//}
//
////=================================================================================================
//// Resets all motion values
//void ResetMotion() {
//    // Angles
//    _xAngle = 0;
//    _yAngle = 0;
//    _zAngle = 0;
//
//    // Position
//    _xPos = 0;
//    _yPos = 0;
//    _zPos = 0;
//}
