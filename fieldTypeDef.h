#include <stdio.h>
#include <stdlib.h>

typedef struct _FieldType
{
    int value;
    int length;
    char* type;
    char* des;
}FieldType

FieldType types
[
    {1, 4, "Incoming counter with length N x 8 bits for number of bytes associated with an IP Flow."},
    {2, 4, "Incoming counter with length N x 8 bits for the number of packets associated with an IP Flow"},
    {3, 4, "Number of flows that were aggregated; default for N is 4"},
    {4, 1, "IP protocol byte"},
    {5, 1, "Type of Service byte setting when entering incoming interface"},
    {6, 1, "Cumulative of all the TCP flags seen for this flow"}
];
