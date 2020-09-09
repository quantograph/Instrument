#pragma once

class PeakMeter;
class SetEffect;
class Slider;
class Button;
class TextBox;
class CheckBox;
class SettingsFile;

class SetGuitar : public Window {
public:
    SetGuitar();

    SettingsFile* _settingsFile{};
    InputSettings* _inputSettings{};
    SetEffect* _setEffect{};
    TextBox* _effect1{};
    TextBox* _effect2{};
    CheckBox* _singleCheck{};
    CheckBox* _doubleCheck{};
    Window* _current{};
    Button* _backButton{};

    bool init(SettingsFile* settingsFile, Settings* settings, Window* parent, ControlId id);
    void setupButtons();
    void updateNumber();

    void draw() override;
    bool onControl(Control* control) override;
    void onBack(Window* window) override;
};
