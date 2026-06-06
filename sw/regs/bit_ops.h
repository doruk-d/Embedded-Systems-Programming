#pragma once
#include <stdint.h>

typedef struct {
    uint8_t pos;
    uint8_t width;
    uint32_t mask;
}reg_field;

#define FIELD(w, p) ((reg_field){.pos = (p), .width = (w), .mask = (((1U << (w)) - 1) << (p))})
#define FIELD_VAL(f, val) (((val) << (f).pos) & (f).mask)
#define FIELD_READ(reg, f) (((reg) & (f).mask) >> (f).pos)
#define FIELD_CLEAR_MASK(f) (~((f).mask))