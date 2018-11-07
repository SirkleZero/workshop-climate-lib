namespace Configuration {
    #ifndef Secrets_h
        #define Secrets_h

        class Secrets {
            private:
                String tmp_adafruitio_username;
                String tmp_adafruitio_key;
                String tmp_wifi_ssid;
                String tmp_wifi_password;
            public:
                // Secrets(){}
                // ~Secrets(){}

                const char *adafruitio_username;
                const char *adafruitio_key;
                const char *wifi_ssid;
                const char *wifi_password;

                void SetUsername(const char * adafruitUsername){
                    this->tmp_adafruitio_username = String(adafruitUsername);
                    this->adafruitio_username = this->tmp_adafruitio_username.c_str();
                }

                void SetPassword(const char * adafruitPassword){
                    this->tmp_adafruitio_key = String(adafruitPassword);
                    this->adafruitio_key = this->tmp_adafruitio_key.c_str();
                }

                void SetWifiSSID(const char * wifiSSID){
                    this->tmp_wifi_ssid = String(wifiSSID);
                    this->wifi_ssid = this->tmp_wifi_ssid.c_str();
                }

                void SetWifiPassword(const char * wifiPassword){
                    this->tmp_wifi_password = String(wifiPassword);
                    this->wifi_password = this->tmp_wifi_password.c_str();
                }

                void PrintDebug(){
                    Serial.println("---------------------------------------");
                    Serial.print("adafruitio_username = "); Serial.println(this->adafruitio_username);

                    Serial.print("adafruitio_key = "); Serial.println(this->adafruitio_key);

                    Serial.print("wifi_ssid = "); Serial.println(this->wifi_ssid);

                    Serial.print("wifi_password = "); Serial.println(this->wifi_password);
                    Serial.println("---------------------------------------");
                    Serial.println();
                }
        };

    #endif
}
