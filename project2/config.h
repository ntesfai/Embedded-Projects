#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED
typedef struct {
    char token[4];
    unsigned int hi_alarm;
    unsigned int hi_warn;
    unsigned int lo_alarm;
    unsigned int lo_warn;
    char use_static_ip;
    unsigned char static_ip[4];
    unsigned char checksum;
} config_struct;

extern config_struct config;
void config_init();
void config_update();
void config_set_modified();
#endif
