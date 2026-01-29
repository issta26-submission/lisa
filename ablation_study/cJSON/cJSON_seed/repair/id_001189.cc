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
//<ID> 1189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    cJSON_AddNumberToObject(stats, "count", 3.0);

    double dvals[3] = {1.1, 2.2, 3.3};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(stats, "doubles", darr);

    float fvals[2] = {4.4f, 5.5f};
    cJSON *farr = cJSON_CreateFloatArray(fvals, 2);
    cJSON_AddItemToObject(stats, "floats", farr);

    cJSON *metrics = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "metrics", metrics);

    // step 2: Configure
    cJSON *m1 = cJSON_CreateObject();
    cJSON_AddNumberToObject(m1, "value", 9.9);
    cJSON_AddItemToArray(metrics, m1);

    cJSON *m2 = cJSON_CreateObject();
    cJSON_AddNumberToObject(m2, "value", 8.8);
    cJSON_AddItemToArray(metrics, m2);

    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    double count = cJSON_GetNumberValue(cJSON_GetObjectItem(stats, "count"));
    cJSON *first_metric = cJSON_GetArrayItem(metrics, 0);
    cJSON *second_metric = cJSON_GetArrayItem(metrics, 1);
    double v1 = cJSON_GetNumberValue(cJSON_GetObjectItem(first_metric, "value"));
    double v2 = cJSON_GetNumberValue(cJSON_GetObjectItem(second_metric, "value"));
    int dcount = cJSON_GetArraySize(darr);
    double final_value = count + v1 + v2 + (double)dcount;
    cJSON_AddNumberToObject(root, "final", final_value);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}