#ifndef Staff_h
#define Staff_h

class Staff {
public:
    int _x = 0;
    int _y = 0;
    int _lineSpace = 0; // Spacing between the staff lines
    int _staffBottom = 0; // Bottom Y of the note staff
    int _middleC = 0; // Y of the middle C line

    Staff();
    /*bool Init(int x, int y);
    void DrawStaff();
    void DrawNote(int note, bool draw);
    float GetNoteLine(int note);
    bool IsSharp(char note);
    void TrebleClef(int x, int y);
    void BassClef(int x, int y);
    void Test();*/
};

#endif
