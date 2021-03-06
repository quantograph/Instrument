#pragma once

class PeakMeter;
class Button;
class SetGuitar;
class SetSynth;
class SetBand;
class SetAudio;
class SettingsFile;

class Main : public Window {
public:
    Main();
    ~Main();

    SetAudio* _setAudio{};
    SetGuitar* _setGuitar{};
    SetSynth* _setSynth{};
    SetBand* _setBand{};
    PeakMeter* _peakMeter{};
    std::vector<Button*> _buttons;
    SettingsFile* _settingsFile{};

    void setupButtons();

    void onPeakMeter(float left, float right) override;
    bool init(SettingsFile* settingsFile, Settings* settings);

    bool onControl(Control* control) override;
    void onBack(Window* window) override;
};
