#include "ControllerConfiguration.h"
#include "Secrets.h"

namespace Configuration {
    #ifndef SDCardProxy_h
        #define SDCardProxy_h

        class SDCardProxy {
            public:
                SDCardProxy();
                void Initialize();
                void LoadSecrets(Secrets *secrets);
                void LoadConfiguration(ControllerConfiguration *configuration);

                void PrintDebug();
            private:
                Secrets secrets;
        };

    #endif
}
