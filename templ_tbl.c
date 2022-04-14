#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "templ_tbl.h"

int init_templ_table()
{
    int i = 0;

    if(!(templ_table = (Templ_cell_t**)malloc(TBL_SIZE * sizeof(Templ_cell_t*))))
    {
        printf("[ ERR] Allocate memory failed\n");
        return FAILED;
    }

    for(i = 0; i < TBL_SIZE; ++i)
    {
        templ_table[i]->src_ip = 0;
        templ_table[i]->templ_id = 0;
        templ_table[i]->ptr_data = NULL;
        templ_table[i]->len = 0;
        templ_table[i]->next = NULL;
    }

    return SUCCESS;
}

int templ_recv(uint32_t src_ip, uint16_t templ_id, uint8_t* ptr_data, uint16_t len)
{
    Templ_cell_t* current = templ_lookup(src_ip, templ_id);

    if((!current) && !(current = (Templ_cell_t*)malloc(sizeof(Templ_cell_t))))
    {
        return FAILED;
    }

    current->src_ip = src_ip;
    current->templ_id = templ_id;
    current->ptr_data = (uint8_t*)malloc(len);
    memcpy(current->ptr_data, ptr_data, len);
    current->len = len;

    return SUCCESS;
}

Templ_cell_t* templ_lookup(uint32_t src_ip, uint16_t templ_id)
{
    uint16_t key = ((src_ip >> 16) & 0xffff) + (src_ip & 0xffff) + templ_id;
    Templ_cell_t* current = templ_table[key];

    while(current)
    {
        if((current->src_ip == src_ip) && (current->templ_id == templ_id))
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

void templ_destruct()
{
    int i = 0;
    for(i = 0; i < TBL_SIZE; ++i)
    {
        Templ_cell_t* current = templ_table[i];

        while(current)
        {
            Templ_cell_t* next = current->next;
            if(current->ptr_data)
            {
                free(current->ptr_data);
            }
            free(current);
            current = next;
        }
    }

    free(templ_table);
    templ_table = NULL;
}
