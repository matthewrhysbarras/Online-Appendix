#include "freertos/event_groups.h"
void check_command(uint8_t* commandstring);
void clear_command_string(void);
void stop_streaming(void);
void SendBuffer(void);

//EventGroupHandle_t event_group;
extern EventGroupHandle_t event_group_dude;

