#include "../Devices/Devices.h"
#include "Effects.h"

//=================================================================================================
Effects::Effects(EffectSettings* settings, AudioStream* source, uint8_t sourceOutput, 
                 AudioStream* dest, uint8_t destInput) :
    _settings(settings), _source(source), _sourceOutput(sourceOutput), _dest(dest), 
    _destInput(destInput) {
}

//=================================================================================================
Effects::~Effects() {
    reset();
}

//=================================================================================================
void Effects::reset() {
}

//=================================================================================================
void Effects::connect(AudioStream* stream) {
    Serial.printf("Effects::connect\n");

    delete _inConnection;
    _inConnection = new AudioConnection(*_source, _sourceOutput, *stream, 0);

    delete _outConnection;
    _outConnection = new AudioConnection(*stream, 0, *_dest, _destInput);
}

//=================================================================================================
// Update settings for the active effect
bool Effects::init(bool reset) {
    const char* title{"Updating effect in Effects::init"};
    Serial.printf("Effects::init: %s (%d), %s\n", _settings, _settings->_effectName.c_str(), 
                  _settings->_effectType, reset ? "connect" : "update");

    switch(_settings->_effectType) {
        case EffectType::eff_clean:
            if(reset) {
                delete _outConnection;
                _outConnection = new AudioConnection(*_source, _sourceOutput, *_dest, _destInput);
            }
            break;

        case EffectType::eff_chorus:
            if(reset)
                connect(&_chorus);

            _settings->_chorus.show(title);
            _chorus.begin(_delayLine, max(DELAY_LINE_LENGTH, _settings->_chorus._delay * AUDIO_BLOCK_SAMPLES), 
                           _settings->_chorus._voices);
            _chorus.voices(_settings->_chorus._voices);

            break;

        case EffectType::eff_flange:
            if(reset)
                connect(&_flange);

            _settings->_flange.show(title);
            _flange.begin(_delayLine, max(DELAY_LINE_LENGTH, _settings->_flange._delay * AUDIO_BLOCK_SAMPLES), 
                          max(DELAY_LINE_LENGTH, _settings->_flange._offset * AUDIO_BLOCK_SAMPLES), 
                          max(DELAY_LINE_LENGTH, _settings->_flange._depth * AUDIO_BLOCK_SAMPLES), 
                          _settings->_flange._rate);

            break;

        case EffectType::eff_reverb:
            if(reset)
                connect(&_reverb);

            _settings->_reverb.show(title);
            _reverb.reverbTime(_settings->_reverb._time);

            break;

        case EffectType::eff_freeverb:
            if(reset)
                connect(&_freeverb);

            _settings->_freeverb.show(title);
            _freeverb.roomsize(_settings->_freeverb._roomSize);
            _freeverb.damping(_settings->_freeverb._damping);

            break;

        case EffectType::eff_envelope:
            if(reset)
                connect(&_envelope);

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
            if(reset)
                connect(&_delay);

            _settings->_delay.show(title);
            for(int i = 0; i < 8; ++i) {
                float delay = _settings->_delay._delays[i];
                if(delay > 0.0)
                    _delay.delay(i, delay);
            }

            break;

        case EffectType::eff_bitcrusher:
            if(reset)
                connect(&_bitcrusher);

            _settings->_bitcrusher.show(title);
    	    _bitcrusher.bits(_settings->_bitcrusher._bits);
            _bitcrusher.sampleRate(_settings->_bitcrusher._rate);

            break;

        case EffectType::eff_waveshaper:
            if(reset)
                connect(&_waveshaper);

            _settings->_waveshaper.show(title);
            _waveshaper.shape(_settings->_waveshaper._waveshape, _settings->_waveshaper._length);

            break;

        case EffectType::eff_granular:
            if(reset)
                connect(&_granular);

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

    return true;
}
