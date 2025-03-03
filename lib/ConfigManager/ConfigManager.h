#ifndef __CONFIG_CONFIG_MANAGER_H__
#define __CONFIG_CONFIG_MANAGER_H__

// constants (changes may lead to crashes)
#define MAX_TIMERS 4                       // maximum number of timers
#define MAX_TIMER_NAME_LENGTH 32           // maximum length of timer name
#define MAX_TIMER_TIME_LENGTH 6            // maximum length of timer time string
#define MAX_WIFI_SSID_LENGTH 32            // maximum length of wifi ssid
#define MAX_WIFI_PASSWORD_LENGTH 64        // maximum length of wifi password
#define MAX_FILENAME_LENGTH 32             // maximum length of filename
#define JSON_BUFFER_SIZE 4096              // size of json buffer
#define DEFAULT_CONFIG_FILE "/config.json" // default config file

#include <Arduino.h>

#ifdef ESP32DEV
#include <SPIFFS.h>
#else
#include <LittleFS.h>
#endif

#include <ArduinoJson.h>

// type definitions
typedef struct {
    unsigned int hour;
    unsigned int minute;
} timer_time_t;

typedef struct {
    char ssid[MAX_WIFI_SSID_LENGTH];
    char password[MAX_WIFI_PASSWORD_LENGTH];
} local_wifi_config_t;

typedef struct {
    char name[MAX_TIMER_NAME_LENGTH];
    timer_time_t time;
    bool enabled;

    bool monday;
    bool tuesday;
    bool wednesday;
    bool thursday;
    bool friday;
    bool saturday;
    bool sunday;
} timer_config_t;

typedef struct {
    timer_config_t* timers;
    size_t num_timers;
} timer_config_list_t;

typedef struct {
    timer_config_list_t timers;
    bool empty;
} optional_timer_config_list_t;

typedef struct {
    int quantity;
} feed_config_t;

typedef struct {
    local_wifi_config_t wifi;
    timer_config_list_t timer_list;
    feed_config_t feed;
} config_t;


class ConfigManager {
    private:
        const char* filename;

    public:
        ConfigManager(const char* filename);
        ConfigManager();
        ~ConfigManager();

        config_t config;

        void load_config();
        void save_config();

        timer_config_list_t sort_timers_by_time(timer_config_list_t timers);
        
        const char* get_wifi_ssid();
        void set_wifi_ssid(const char* ssid);
        const char* get_wifi_password();
        void set_wifi_password(const char* password);

        timer_config_t get_timer(int index);
        size_t get_num_timers();
        timer_config_list_t get_timers();
        StaticJsonDocument<JSON_BUFFER_SIZE> get_timers_json();
        feed_config_t get_feed_config();

        void set_timers_json(JsonVariant &json);

        String time_to_string(timer_time_t time);
        timer_time_t get_time_from_string(String time);

        // debugging functions
        void print_config();
        void print_timers();
};

#endif