#pragma once

struct EffectSettings;

class Effects {
    struct Connection {
        AudioConnection* _in{nullptr};
        AudioConnection* _out{nullptr};
        bool _connected{false};
        String _name{};
        EffectType _type{EffectType::eff_none};

        Connection(const char* name, EffectType type) : 
            _name(name), _type(type) {
        }

        void connect() {
            if(_in)
                _in->connect();

            if(_out)
                _out->connect();

            _connected = true;
        }

        void disconnect() {
            if(_in)
                _in->disconnect();

            if(_out)
                _out->disconnect();

            _connected = false;
        }

        void show(const char* title) {
            Serial.printf("%s: connection '%s', conn=%s\n", title, _name.c_str(), _connected ? "Y" : "N");
        }
    };

    #define CHORUS_DELAY_LENGTH (16*AUDIO_BLOCK_SAMPLES)
    #define FLANGE_DELAY_LENGTH (6*AUDIO_BLOCK_SAMPLES)

    #define DELAY_LINE_LENGTH (64*AUDIO_BLOCK_SAMPLES)
    short _delayLine[DELAY_LINE_LENGTH];

public:
    Effects(EffectSettings* settings, AudioStream* source, uint8_t sourceOutput, 
            AudioStream* dest, uint8_t destInput);
    ~Effects();
    bool init();
    void makeConnection(EffectType type, AudioStream* effect);
    void disconnectAll();
    bool connect(EffectType type);
    void showConnections(const char* title, bool showAll = false);
    bool update();
    static std::vector<String> _effectName;

    EffectSettings* _settings{};
    AudioStream* _source{nullptr};
    uint8_t _sourceOutput{0};
    AudioStream* _dest{nullptr};
    uint8_t _destInput{0};
    int16_t* _granularMemory{nullptr};
    std::vector<Connection*> _connections; // All in/out connecions for all effects
    Connection* _currConnection{nullptr};
    // Effects
    AudioEffectChorus _chorus;
    AudioEffectFlange _flange;
    AudioEffectReverb _reverb;
    AudioEffectFreeverb _freeverb;
    AudioEffectEnvelope _envelope;
    AudioEffectDelay _delay;
    AudioEffectBitcrusher _bitcrusher;
    AudioEffectWaveshaper _waveshaper;
    AudioEffectGranular _granular;
};
