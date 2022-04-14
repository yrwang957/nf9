#include <stdio.h>
#include <stdlib.h>

#include "templ_tbl.h"

int init_templ_table()
{
    int i = 0;

    if((templ_table = (Templ_cell_t**)malloc(TBL_SIZE * sizeof(Templ_cell_t*))) == NULL)
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

int templ_recv(uint32_t src_ip, uint16_t templ_id, uint8_t* ptr_data, int len)
{
    uint16_t key = ((src_ip>>16) & 0xffff) + (src_ip & 0xffff) + templ_id;
    Templ_cell_t* current = templ_table[key];

    while(current)
    {
        if((current->next->src_ip == src_ip) && (current->next->templ_id == templ_id))
        {
            break;
        }
        current = current->next;
    }

    current = (Templ_cell_t*)malloc(sizeof(Templ_cell_t));
    current->next = NULL;

    return SUCCESS;
}

Templ_cell_t* templ_lookup(uint32_t src_ip, uint16_t templ_id)
{
    return NULL;
}

void templ_destory()
{
    int i = 0;
    for(i = 0; i < TBL_SIZE; ++i)
    {
        Templ_cell_t* current = templ_table[i];

        while(current)
        {
            Templ_cell_t* next = current->next;
            free(current);
            current = next;
        }
    }

    free(templ_table);
}
