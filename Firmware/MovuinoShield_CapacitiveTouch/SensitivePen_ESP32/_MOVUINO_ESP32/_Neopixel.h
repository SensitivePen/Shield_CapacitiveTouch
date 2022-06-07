#if !defined(_MOVUINO_NEOPIXEL_)
#define _MOVUINO_NEOPIXEL_

#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIX 15

class MovuinoNeopixel
{
private:
    Adafruit_NeoPixel _pix;
    unsigned long _timeShow0;
    int _brightness = 255;
    int _color;

    uint8_t getRed(uint32_t color_);
    uint8_t getGreen(uint32_t color_);
    uint8_t getBlue(uint32_t color_);

    // Blink animation
    bool _isBlinking = false;
    unsigned long _timeBlink0;
    int _timeBlinkOn;
    int _timeBlinkOff;
    int _nBlink;

    // Breath animation
    bool _isBreathing = false;
    float _breathIntens = 1.0f;
    int _timeBreath;

    // Rainbow animation
    bool _isRainbow = false;
    long _rainbowHue = 0;

public:
    MovuinoNeopixel();
    ~MovuinoNeopixel();

    void begin();
    void update();
    void forceUpdate();

    // Setters
    void turnOff();
    void turnOn();
    void setColor(uint32_t color_);
    void setColor(uint8_t red_, uint8_t green_, uint8_t blue_);
    void setBrightness(int bright_);
    void lerpTo(uint32_t toColor_, float intensity_);

    // Animations
    void breathOn(int periodMs_);
    void breathOn(int periodMs_, float intensity_);
    void breathOff();
    void blinkOn(int timeOn_);
    void blinkOn(int timeOn_, int nBlink_);
    void asyncBlinkOn(int timeOn_, int timeOff_);
    void asyncBlinkOn(int timeOn_, int timeOff_, int nBlink_);
    void blinkOff();
    void rainbowOn();
    void rainbowOff();

    // Getters
    uint32_t getColor();
};

#endif // _MOVUINO_NEOPIXEL_