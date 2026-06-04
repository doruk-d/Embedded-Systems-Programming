#ifndef DWT_H
#define DWT_H

typedef enum{
    SUPPORTED_CYCLE_COUNTER = 0,
    UNSUPPORTED_CYCLE_COUNTER
}cyc_cnt_t;

cyc_cnt_t dwt_init(void);

#endif