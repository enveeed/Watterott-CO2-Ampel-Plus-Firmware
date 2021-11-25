// timer

// timer: mode
static const int TIMER_MODE_OFF = 0;    // feature is always off
static const int TIMER_MODE_ON = 1;     // feature is always on
static const int TIMER_MODE_TIMER = 2;  // feature is timer-controlled

// timer: init and load config values
void timer_init();

// timer: loop
void timer_loop();