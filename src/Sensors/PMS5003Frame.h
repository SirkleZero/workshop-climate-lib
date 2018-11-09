#ifndef PMS5003Frame_h
    #define PMS5003Frame_h

    namespace Sensors {
        struct PMS5003Frame {
            uint8_t  frameHeader[2];
            uint16_t frameLen;
            ParticleData particulates;
            uint8_t  version;
            uint8_t  errorCode;
            uint16_t checksum;
        };
    }
#endif
    