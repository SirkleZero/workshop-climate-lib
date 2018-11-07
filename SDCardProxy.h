#include "Secrets.h"

namespace Configuration {
    #ifndef SDCardProxy_h
        #define SDCardProxy_h

        class SDCardProxy {
            public:
                SDCardProxy();
                void Initialize();
                void LoadSecrets(Secrets *secrets);
                void PrintDebug();
            private:
                Secrets secrets;
        };

    #endif
}
