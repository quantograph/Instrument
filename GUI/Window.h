#ifndef Window_h
#define Window_h

class Window {
public:
    Window(Settings::Data* settings);
    ~Window();

    Settings::Data* _settings{};
    std::vector<Control*> _controls{};

    virtual void draw();
    virtual bool init() { return true; };
};

#endif // Window_h
