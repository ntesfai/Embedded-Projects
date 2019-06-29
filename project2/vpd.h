#ifndef VPD_H_INCLUDED
#define VPD_H_INCLUDED
typedef struct{
    char token[4];
    char model[12];
    char manufacturer[12];
    char serial_number[12];
    unsigned long manufacture_date;
    unsigned char mac_address[6];
    char country_of_origin[4];
    unsigned char checksum;
} vpd_struct;
extern vpd_struct vpd;
void vpd_init();
#endif
