
/**************************************************************************/
/*!
  @brief  Wave Pattern. Sinusoidal motion with continuous depth variation.
  Sensation controls the wave period (number of strokes per full cycle).
*/
/**************************************************************************/
class Wave : public Pattern {
  public:
    Wave(const char *str) : Pattern(str) {}

    void setTimeOfStroke(float speed = 0) {
        _timeOfStroke = 0.5 * speed;
    }

    void setSensation(float sensation) {
        _sensation = sensation;
        _wavePeriod = map(sensation, -100, 100, 2, 20);
    }

    motionParameter nextTarget(unsigned int index) {
        float cyclePos = (float)((index / 2) % _wavePeriod) / (float)_wavePeriod;
        float depthFactor = 0.65 + 0.35 * sin(2.0 * PI * cyclePos);
        int waveStroke = (int)((float)_stroke * depthFactor);

        _nextMove.speed = int(1.5 * waveStroke / _timeOfStroke);
        _nextMove.acceleration = int(3.0 * _nextMove.speed / _timeOfStroke);

        if (index % 2) {
            _nextMove.stroke = _depth - _stroke;
        } else {
            _nextMove.stroke = (_depth - _stroke) + waveStroke;
        }

        _index = index;
        return _nextMove;
    }

  protected:
    int _wavePeriod = 8;
};

/**************************************************************************/
/*!
  @brief  Random Pattern. Strokes with random depth within a defined range.
  Sensation controls the randomness range.
*/
/**************************************************************************/
class RandomPattern : public Pattern {
  public:
    RandomPattern(const char *str) : Pattern(str) {}

    void setTimeOfStroke(float speed = 0) {
        _timeOfStroke = 0.5 * speed;
    }

    void setSensation(float sensation) {
        _sensation = sensation;
        _randomRange = map(abs(sensation), 0, 100, 10, 100);
    }

    motionParameter nextTarget(unsigned int index) {
        int minStroke = _stroke * (100 - _randomRange) / 100;
        int randomStroke = random(minStroke, _stroke + 1);

        float speedVariation = 0.7 + (float)random(0, 60) / 100.0;
        float adjustedTime = _timeOfStroke * speedVariation;

        _nextMove.speed = int(1.5 * randomStroke / adjustedTime);
        _nextMove.acceleration = int(3.0 * _nextMove.speed / adjustedTime);

        if (index % 2) {
            _nextMove.stroke = _depth - _stroke;
        } else {
            _nextMove.stroke = (_depth - _stroke) + randomStroke;
        }

        _index = index;
        return _nextMove;
    }

  protected:
    int _randomRange = 50;
};

/**************************************************************************/
/*!
  @brief  Accelerando Pattern. Speed gradually increases over a series of
  strokes then resets. Sensation controls the number of strokes in the ramp.
*/
/**************************************************************************/
class Accelerando : public Pattern {
  public:
    Accelerando(const char *str) : Pattern(str) {}

    void setTimeOfStroke(float speed = 0) {
        _timeOfStroke = 0.5 * speed;
    }

    void setSensation(float sensation) {
        _sensation = sensation;
        _rampLength = map(sensation, -100, 100, 4, 24);
    }

    motionParameter nextTarget(unsigned int index) {
        int cycleIndex = (index / 2) % _rampLength;
        float speedFactor = 0.5 + (1.0 * (float)cycleIndex / (float)(_rampLength - 1));
        float adjustedTime = _timeOfStroke / speedFactor;

        _nextMove.speed = int(1.5 * _stroke / adjustedTime);
        _nextMove.acceleration = int(3.0 * _nextMove.speed / adjustedTime);

        if (index % 2) {
            _nextMove.stroke = _depth - _stroke;
        } else {
            _nextMove.stroke = _depth;
        }

        _index = index;
        return _nextMove;
    }

  protected:
    int _rampLength = 12;
};

/**************************************************************************/
/*!
  @brief  Pulse Pattern. Small fast strokes at shallow depth producing a
  vibrating effect. Sensation controls the pulse depth.
*/
/**************************************************************************/
class Pulse : public Pattern {
  public:
    Pulse(const char *str) : Pattern(str) {}

    void setTimeOfStroke(float speed = 0) {
        _timeOfStroke = 0.5 * speed;
    }

    void setSensation(float sensation) {
        _sensation = sensation;
        _pulseFraction = map(sensation, -100, 100, 5, 30) / 100.0;
    }

    motionParameter nextTarget(unsigned int index) {
        int pulseStroke = (int)((float)_stroke * _pulseFraction);
        float pulseTime = _timeOfStroke / 3.0;

        _nextMove.speed = int(1.5 * pulseStroke / pulseTime);
        _nextMove.acceleration = int(3.0 * _nextMove.speed / pulseTime);

        if (index % 2) {
            _nextMove.stroke = _depth - pulseStroke;
        } else {
            _nextMove.stroke = _depth;
        }

        _index = index;
        return _nextMove;
    }

  protected:
    float _pulseFraction = 0.15;
};

/**************************************************************************/
/*!
  @brief  Sawtooth Pattern. Depth ramps up linearly then drops back
  quickly. Sensation controls the ramp steepness.
*/
/**************************************************************************/
class Sawtooth : public Pattern {
  public:
    Sawtooth(const char *str) : Pattern(str) {}

    void setTimeOfStroke(float speed = 0) {
        _timeOfStroke = 0.5 * speed;
    }

    void setSensation(float sensation) {
        _sensation = sensation;
        _rampSteps = map(sensation, -100, 100, 3, 15);
    }

    motionParameter nextTarget(unsigned int index) {
        int cycleIndex = (index / 2) % _rampSteps;
        int sawStroke = _stroke * (cycleIndex + 1) / _rampSteps;

        _nextMove.speed = int(1.5 * sawStroke / _timeOfStroke);
        _nextMove.acceleration = int(3.0 * _nextMove.speed / _timeOfStroke);

        if (index % 2) {
            _nextMove.stroke = _depth - _stroke;
        } else {
            _nextMove.stroke = (_depth - _stroke) + sawStroke;
        }

        _index = index;
        return _nextMove;
    }

  protected:
    int _rampSteps = 8;
};

/**************************************************************************/
/*!
  @brief  Heartbeat Pattern. Two fast strokes followed by a pause,
  mimicking a heartbeat rhythm. Sensation controls pause length.
*/
/**************************************************************************/
class Heartbeat : public Pattern {
  public:
    Heartbeat(const char *str) : Pattern(str) {}

    void setTimeOfStroke(float speed = 0) {
        _timeOfStroke = 0.5 * speed;
    }

    void setSensation(float sensation) {
        _sensation = sensation;
        _pauseDuration = map(sensation, -100, 100, 200, 3000);
        _updateDelay(_pauseDuration);
    }

    motionParameter nextTarget(unsigned int index) {
        int beatIndex = (index / 2) % 3;

        if (beatIndex == 2) {
            if (_isStillDelayed() == false) {
                _startDelay();
            }
            _nextMove.skip = _isStillDelayed();
        } else {
            _nextMove.skip = false;
            float beatTime = _timeOfStroke / 2.0;
            _nextMove.speed = int(1.5 * _stroke / beatTime);
            _nextMove.acceleration = int(3.0 * _nextMove.speed / beatTime);
        }

        if (!_nextMove.skip) {
            if (index % 2) {
                _nextMove.stroke = _depth - _stroke;
            } else {
                _nextMove.stroke = _depth;
            }
        }

        _index = index;
        return _nextMove;
    }

  protected:
    int _pauseDuration = 1000;
};

/**************************************************************************/
/*!
  @brief  Edge Pattern. Maintains deep position with small movements.
  Sensation controls the size of the small movements.
*/
/**************************************************************************/
class Edge : public Pattern {
  public:
    Edge(const char *str) : Pattern(str) {}

    void setTimeOfStroke(float speed = 0) {
        _timeOfStroke = 0.5 * speed;
    }

    void setSensation(float sensation) {
        _sensation = sensation;
        _edgeFraction = map(sensation, -100, 100, 3, 20) / 100.0;
    }

    motionParameter nextTarget(unsigned int index) {
        int edgeStroke = (int)((float)_stroke * _edgeFraction);

        _nextMove.speed = int(1.5 * edgeStroke / _timeOfStroke);
        _nextMove.acceleration = int(3.0 * _nextMove.speed / _timeOfStroke);

        if (index % 2) {
            _nextMove.stroke = (_depth - _stroke) + (_stroke - edgeStroke);
        } else {
            _nextMove.stroke = _depth;
        }

        _index = index;
        return _nextMove;
    }

  protected:
    float _edgeFraction = 0.1;
};
