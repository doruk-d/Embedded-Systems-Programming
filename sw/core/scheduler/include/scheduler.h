#pragma once
#include "common.h"

typedef enum {
    SYS_YIELD
}svc_call_t;

system_status_t task_create(void *arg, void (*task_func)(void*));
void yield(void);
void scheduler_init(void);
void scheduler_run(void);