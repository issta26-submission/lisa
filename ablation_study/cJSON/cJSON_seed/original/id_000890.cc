#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 890
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double dnums[3] = {1.5, 2.5, 3.0};
    float fnums[2] = {4.0f, 6.0f};
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *darr = cJSON_CreateDoubleArray(dnums, 3);
    cJSON_AddItemToObject(data, "double_array", darr);
    cJSON *farr = cJSON_CreateFloatArray(fnums, 2);
    cJSON_AddItemToObject(data, "float_array", farr);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buf = cJSON_malloc(128);
    memset(buf, 0, 128);
    char *prebuf = (char *)buf;
    cJSON_PrintPreallocated(root, prebuf, 128, 0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "format", cJSON_CreateString("v1"));

    // step 3: Operate
    int dcount = cJSON_GetArraySize(darr);
    double a0 = cJSON_GetNumberValue(cJSON_GetArrayItem(darr, 0));
    double a1 = cJSON_GetNumberValue(cJSON_GetArrayItem(darr, 1));
    double a2 = cJSON_GetNumberValue(cJSON_GetArrayItem(darr, 2));
    double dsum = a0 + a1 + a2;
    double davg = dsum / (double)dcount;
    cJSON_AddNumberToObject(data, "double_avg", davg);
    double f0 = cJSON_GetNumberValue(cJSON_GetArrayItem(farr, 0));
    double f1 = cJSON_GetNumberValue(cJSON_GetArrayItem(farr, 1));
    double favg = (f0 + f1) / 2.0;
    cJSON_AddNumberToObject(data, "float_avg", favg);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(data, "float_array");
    cJSON_free(snapshot);
    cJSON_free(prebuf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}