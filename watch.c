#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>

#include "watch.h"
#include "buffer.h"
#include "nf9r.h"

int initWatch()
{
    //TODO:  move to run? race condition?
    pthread_create(&tw, NULL, (void*)watch, NULL);

    return SUCCESS;
}

void printBuf()
{
    //test print
        printf("Buffer:\n");
        int it;
        for(it = 0; it < BUF_SIZE; ++it)
        {
            if(bs[it].id == -1)
                continue;

            printf("%03d: id %d, dur time %d, len %d, type %d\n", it, bs[it].id, (int)time(NULL) - (int)bs[it].time, bs[it].length, bs[it].type);

            if(bs[it].type == BUF_TMPLATE)
            {
                TemplateFlowSet* t = (TemplateFlowSet*)bs[it].p;
                printf("  id %u, c %u\n", ntohs(t->templateId), ntohs(t->fieldCount));

                char* p = (char*)t + 4;
                int it2;
                for(it2 = 0; it2 < ntohs(t->fieldCount); ++it2)
                {
                    int type = (*p << 8) | *(p + 1);
                    int leng = (*(p + 2) << 8) | *(p + 3);
                    printf("    %3d: type %3d, leng %3d\n", it2, type, leng);
                    p += 4;
                }
                printf("\n");
            }

            if(bs[it].type == BUF_OTEMPLATE)
            {
                OptionsTemplate* t = (OptionsTemplate*)bs[it].p;
                printf("  id %u, optSLeng %u, optLeng %u\n", ntohs(t->templateId), ntohs(t->optionScopeLength), ntohs(t->optionsLength));

                char* p = (char*)t + 6;

                //Scope Field
                int it2;
                for(it2 = 0; it2 < ntohs(t->optionScopeLength); it += 4)
                {
                    int type = (*p << 8) | *(p + 1);
                    int leng = (*(p + 2) << 8) | *(p + 3);
                    printf("    %3d: sf type %3d, sf leng %3d\n", it2, type, leng);
                    p += 4;
                }
                //Option Field(not include scope field)
                int it3;
                for(it3 = 0; it3 < ntohs(t->optionScopeLength); it3 += 4)
                {
                    int type = (*p << 8) | *(p + 1);
                    int leng = (*(p + 2) << 8) | *(p + 3);
                    printf("    %3d: of type %3d, of leng %3d\n", it3, type, leng);
                    p += 4;
                }
                printf("\n");
            }

            if(bs[it].type == BUF_DATA)
            {
                Data* t = (Data*)bs[it].p;
                printf("  id %u, l %u\n\n", ntohs(t->flowSetId), ntohs(t->length));
            }
        }
        printf("\n");
        //
}

void watch()
{
    struct timeval tv;

    for(;;)
    {
        tv.tv_sec = 10;
        tv.tv_usec = 100000; //1000000 = 1sec
        select(0, NULL, NULL, NULL, &tv);

        //+---------------+
        //| Do somethings |
        //+---------------+


        //test print
        printBuf();
    }
}
