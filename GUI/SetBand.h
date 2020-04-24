#pragma once

class PeakMeter;
class Slider;
class Button;
class TextBox;
class CheckBox;
class SettingsFile;

class SetBand : public Window {
public:
    SetBand();

    SettingsFile* _settingsFile{};
    Button* _backButton{};
    char _string[32];

    bool init(SettingsFile* settingsFile, Settings* settings, Window* parent, ControlId id);
    void setupButtons();

    bool onControl(Control* control) override;
};
