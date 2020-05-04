#include "../Devices/Devices.h"
#include "Effects.h"

// Sound effect names
std::vector<String> Effects::_effectName {
    "none",
    "clean",
    "chorus",
    "flange",
    "reverb",
    "freeverb",
    "envelope",
    "delay",
    "bitcrusher",
    "waveshaper",
    "granular"
};

//=================================================================================================
Effects::Effects(EffectSettings* settings, AudioStream* source, uint8_t sourceOutput, 
                 AudioStream* dest, uint8_t destInput) :
    _settings(settings), _source(source), _sourceOutput(sourceOutput), _dest(dest), 
    _destInput(destInput) {

    memset(_delayLine, 0, DELAY_LINE_LENGTH); // To prevent noise on the first use
}

//=================================================================================================
Effects::~Effects() {
}

//=================================================================================================
bool Effects::init() {
    _connections.resize(eff_last);

    _connections[EffectType::eff_none] = nullptr; // To align the vector with type number
    makeConnection(eff_clean, nullptr);
    makeConnection(eff_chorus, &_chorus);
    makeConnection(eff_flange, &_flange);
    makeConnection(eff_reverb, &_reverb);
    makeConnection(eff_freeverb, &_freeverb);
    makeConnection(eff_envelope, &_envelope);
    makeConnection(eff_delay, &_delay);
    makeConnection(eff_bitcrusher, &_bitcrusher);
    makeConnection(eff_waveshaper, &_waveshaper);
    makeConnection(eff_granular, &_granular);

    showConnections("init", true);

    return true;
}

//=================================================================================================
void Effects::makeConnection(EffectType type, AudioStream* effect) {
    Connection* connection = new Connection(_effectName[type].c_str(), type);

    if(type == EffectType::eff_clean) {
        connection->_out = new AudioConnection(*_source, _sourceOutput, *_dest, _destInput);
    } else {
        connection->_in = new AudioConnection(*_source, _sourceOutput, *effect, 0);
        connection->_out = new AudioConnection(*effect, 0, *_dest, _destInput);
    }
    
    connection->disconnect();
    _connections[type] = connection;
}

//=================================================================================================
void Effects::disconnectAll() {
    for(auto connection : _connections) {
        connection->disconnect();
    }
}

//=================================================================================================
bool Effects::connect(EffectType type) {
    // Don't reconnect the same connection
    if(_currConnection->_type == type) {
        Serial.printf("%s already connected\n", _currConnection->_name.c_str());
        return true;
    }

    // Find the connection to connect
    disconnectAll();
    Connection* connection{_connections[type]};
    if(!connection) {
        Serial.printf("##### ERROR: connection %s (%d) not found\n", _effectName[type], type);
        return false;
    }

    connection->connect();
    _currConnection = connection;
    Serial.printf("Connected %s (%d)\n", _effectName[type].c_str(), type);

    return true;
}

//=================================================================================================
void Effects::showConnections(const char* title, bool showAll) {
    Serial.printf("%s: %d connecions -------------\n", title, _connections.size());

    for(auto connection : _connections) {
        if(connection->_connected || showAll)
            connection->show(title);
    }

    Serial.printf("-------------------------\n");
}

//=================================================================================================
// Update settings for the active effect
bool Effects::update() {
    const char* title{"Effects::update"};
    Serial.printf("Effects::update: %s (%d) >>>>>>>>>>>>\n",
                  _settings->_effectName.c_str(), _settings->_effectType);

    connect(_settings->_effectType);

    switch(_settings->_effectType) {
        case EffectType::eff_clean:
            break;

        case EffectType::eff_chorus:
            _settings->_chorus.show(title);
            _chorus.begin(_delayLine, max(DELAY_LINE_LENGTH, _settings->_chorus._delay * AUDIO_BLOCK_SAMPLES), 
                          _settings->_chorus._voices);
            _chorus.voices(_settings->_chorus._voices);
            break;

        case EffectType::eff_flange:
            _settings->_flange.show(title);
            _flange.begin(_delayLine, max(DELAY_LINE_LENGTH, _settings->_flange._delay * AUDIO_BLOCK_SAMPLES), 
                          max(DELAY_LINE_LENGTH, _settings->_flange._offset * AUDIO_BLOCK_SAMPLES), 
                          max(DELAY_LINE_LENGTH, _settings->_flange._depth * AUDIO_BLOCK_SAMPLES), 
                          _settings->_flange._rate);
            break;

        case EffectType::eff_reverb:
            _settings->_reverb.show(title);
            _reverb.reverbTime(_settings->_reverb._time);
            break;

        case EffectType::eff_freeverb:
            _settings->_freeverb.show(title);
            _freeverb.roomsize(_settings->_freeverb._roomSize);
            _freeverb.damping(_settings->_freeverb._damping);
            break;

        case EffectType::eff_envelope:
            _settings->_envelope.show(title);
	        _envelope.delay(_settings->_envelope._delay);
	        _envelope.attack(_settings->_envelope._attack);
	        _envelope.hold(_settings->_envelope._hold);
	        _envelope.decay(_settings->_envelope._decay);
	        _envelope.sustain(_settings->_envelope._sustain);
	        _envelope.release(_settings->_envelope._release);
	        _envelope.releaseNoteOn(_settings->_envelope._releaseNoteOn);
            break;

        case EffectType::eff_delay:
            _settings->_delay.show(title);
            for(int i = 0; i < 8; ++i) {
                float delay = _settings->_delay._delays[i];
                if(delay > 0.0)
                    _delay.delay(i, delay);
            }
            break;

        case EffectType::eff_bitcrusher:
            _settings->_bitcrusher.show(title);
    	    _bitcrusher.bits(_settings->_bitcrusher._bits);
            _bitcrusher.sampleRate(_settings->_bitcrusher._rate);
            break;

        case EffectType::eff_waveshaper:
            _settings->_waveshaper.show(title);
            _waveshaper.shape(_settings->_waveshaper._waveshape, _settings->_waveshaper._length);
            break;

        case EffectType::eff_granular:
            _settings->_granular.show(title);
            _granular.begin(_granularMemory, GRANULAR_MEMORY_SIZE);
	        _granular.setSpeed(_settings->_granular._ratio);
	        _granular.beginFreeze(_settings->_granular._freeze);
	        _granular.beginPitchShift(_settings->_granular._shift);
            break;

        default:
            Serial.printf("##### ERROR: Unknown effect type: %d\n", _settings->_effectType);
            return false;
    }

    showConnections(title);

    Serial.printf("<<<<<<<<<<<<<< Effects::update\n");
    return true;
}
