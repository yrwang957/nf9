#ifndef TEMPL_TBL_H
#define TEMPL_TBL_H

#include <stdint.h>
#include "def.h"

#define TBL_SIZE (65535)

typedef struct Templ_cell_s Templ_cell_t;
struct Templ_cell_s
{
    uint32_t src_ip;
    uint16_t templ_id;
    uint8_t* ptr_data;
    uint16_t len;
    Templ_cell_t* next;
};

Templ_cell_t** templ_table;

int init_templ_table();
int templ_recv(uint32_t src_ip, uint16_t templ_id, uint8_t* ptr_data, int len);
Templ_cell_t* templ_lookup(uint32_t src_ip, uint16_t templ_id);
void templ_destory();

#endif /* TEMPL_TBL_H */
