#ifndef BUZZER_H
#define BUZZER_H

extern bool buzzer_enabled;

void buzzer_init();
void buzzer_on();
void buzzer_off();
void buzzer_test();
void buzzer_ack();

#endif
