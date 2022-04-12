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
#include "fieldTypeDef.h"

void printBuf();
void printDataJson();
void cleanOldBuf();

bool _haveBufferContent();
bool _haveJsonData();

int initWatch()
{
    //TODO:  move to run? bareful race condition?
    pthread_create(&tw, NULL, (void*)watch, NULL);

    return SUCCESS;
}

void printBuf()
{
    printf("Buffer:\n");
    int it;
    for(it = 0; it < BUF_SIZE; ++it)
    {
        if(bs[it].id == -1)
            continue;

        printf("%03d: id %d, dur time %d, len %d, type %d\n", it, bs[it].id, (int)time(NULL) - (int)bs[it].time, bs[it].length, bs[it].type);

        if(bs[it].type == BUF_TEMPLATE)
        {
            TemplateFlowSet* t = (TemplateFlowSet*)bs[it].p;
            printf("  id %u, c %u\n", ntohs(t->templateId), ntohs(t->fieldCount));

            char* p = (char*)t + 4;
            int it2;
            for(it2 = 0; it2 < ntohs(t->fieldCount); ++it2)
            {
                int type = (*p << 8) | *(p + 1);
                int leng = (*(p + 2) << 8) | *(p + 3);
                printf("    %3d: type %d(%s), leng %d\n", it2, type, v9_v10_template_types[type - 1].fieldType , leng);
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
}

//+----------------------+
//| Print Data with json |
//+----------------------+
void printJsonData()
{
    int dataCounter = 0;
    printf("Json:\n");
    printf("{");

    //find Data
    int i = 0;
    for(i = 0; i < BUF_SIZE; ++i)
    {
        if((bs[i].type == BUF_DATA) && (bs[i].id != -1) && (bs[i].using == false))
        {
            int id = bs[i].id;
            //find Template
            int j = 0;
            for(j = 0; j < BUF_SIZE; ++j)
            {
                if((bs[j].type == BUF_TEMPLATE) && (bs[j].id == id) && (bs[j].using == false))
                {
                    printf("%s", dataCounter ? "," : "");
                    ++dataCounter;
                    printf("\n    \"%d\" :\n    [", id);

                    //a pair{Template, Data}, Data have one or more record
                    TemplateFlowSet* t = (TemplateFlowSet*)bs[j].p;
                    int fieldCount = ntohs(t->fieldCount);

                    Data* d = (Data*)bs[i].p;
                    int dLength = ntohs(d->length);

                    //locate Data pointer
                    char* pd = bs[i].p + sizeof(Data);

                    //each record in data
                    int pLength = 0;
                    int recordCounter = 0;
                    for(recordCounter = 0; ; ++recordCounter)
                    {
                        //Template place reset
                        char* pt = bs[j].p + sizeof(TemplateFlowSet);

                        printf("%s\n", recordCounter ? "," : "");
                        printf("        {");

                        int fieldCounter = 0;
                        for(fieldCounter = 0; fieldCounter < fieldCount; ++fieldCounter)
                        {
                            int tt = (*pt << 8) | *(pt + 1);
                            int tl = (*(pt + 2) << 8) | *(pt + 3);
                            printf("%s", fieldCounter ? "," : "");
                            printf("\n            \"%s(%d|%d)\" : ", v9_v10_template_types[tt - 1].fieldType, tt, tl);

                            printf("\"");
                            for(int k = 0; k < tl; ++k)
                            {
                                printf("%c", k ? ' ' : '\0');
                                printf("%02x", pd[k]&0xff);
                            }
                            printf("\"");

                            pt += 4;
                            pd += tl;
                            pLength += tl;
                        }
                        printf("\n        }");

                        if(dLength - pLength < 4)
                            break;
                    }
                    printf("\n    ]");

                    eraseBuf(&bs[i]);
                }
            }
        }
    }

    printf("\n}\n\n");
}

//+--------------------+
//| Delete too old buf |
//+--------------------+
void cleanOldBuf()
{
    int i = 0;
    for(i = 0; i < BUF_SIZE; ++i)
    {
        if((bs[i].type == BUF_DATA) && (bs[i].id != -1) && (bs[i].using == false))
        {
            if(time(NULL) - bs[i].time > 600)
            {
                eraseBuf(&bs[i]);
            }
        }
    }
}

bool _haveJsonData()
{
    int i = 0;
    for(i = 0; i < BUF_SIZE; ++i)
    {
        if((bs[i].type == BUF_DATA) && (bs[i].id != -1) && (bs[i].using == false))
        {
            int id = bs[i].id;
            int j = 0;
            for(j = 0; j < BUF_SIZE; ++j)
            {
                if((bs[j].type == BUF_TEMPLATE) && (bs[j].id == id) && (bs[j].using == false))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool _haveBufferContent()
{
    int i = 0;
    for(i = 0; i < BUF_SIZE; ++i)
    {
        if((bs[i].id != -1) && (bs[i].using == false))
        {
            return true;
        }
    }

    return false;
}

void watch()
{
    struct timeval tv;

    for(;;)
    {
        tv.tv_sec = 5;
        tv.tv_usec = 0; //1000000 = 1sec
        select(0, NULL, NULL, NULL, &tv);

        if(_haveBufferContent())
        {
            printBuf();
        }

        if(_haveJsonData())
        {
            printJsonData();
        }

        cleanOldBuf();
    }
}
