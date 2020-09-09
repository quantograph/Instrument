// Motion.h - all definitons for motion detection

int _motionCount = 0; // Count of the motion reading

// Acceleration values
int16_t _min_ax = 0;
int16_t _min_ay = 0;
int16_t _min_az = 0;

int16_t _max_ax = 0;
int16_t _max_ay = 0;
int16_t _max_az = 0;

// Gyroscope values
int16_t _gxPrev = MAX_INT16;
int16_t _gyPrev = MAX_INT16;
int16_t _gzPrev = MAX_INT16;

int16_t _gxChange = 0;
int16_t _gyChange = 0;
int16_t _gzChange = 0;

int16_t _gxDrift = -88;
int16_t _gyDrift = -258;
int16_t _gzDrift = 16;

int _gxTotal = 0;
int _gyTotal = 0;
int _gzTotal = 0;

int16_t _min_gx = 0;
int16_t _min_gy = 0;
int16_t _min_gz = 0;

int16_t _max_gx = 0;
int16_t _max_gy = 0;
int16_t _max_gz = 0;

// Angles
long _xAngle = 0;
long _yAngle = 0;
long _zAngle = 0;

// Position
int32_t _xPos = 0;
int32_t _yPos = 0;
int32_t _zPos = 0;
