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
//<ID> 895
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double dvals[3] = {1.5, 2.5, 3.5};
    float fvals[2] = {4.5f, 5.5f};
    cJSON *darray = cJSON_CreateDoubleArray(dvals, 3);
    cJSON *farray = cJSON_CreateFloatArray(fvals, 2);
    cJSON_AddItemToObject(root, "doubles", darray);
    cJSON_AddItemToObject(root, "floats", farray);

    // step 2: Configure
    cJSON *stats = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "stats", stats);
    const char *ver = cJSON_Version();
    cJSON_AddStringToObject(root, "version", ver);

    // step 3: Operate
    cJSON *d0 = cJSON_GetArrayItem(darray, 0);
    cJSON *d2 = cJSON_GetArrayItem(darray, 2);
    double sum_doubles = cJSON_GetNumberValue(d0) + cJSON_GetNumberValue(d2);
    cJSON_AddNumberToObject(stats, "sum_doubles", sum_doubles);
    cJSON *f0 = cJSON_GetArrayItem(farray, 0);
    cJSON *f1 = cJSON_GetArrayItem(farray, 1);
    double sum_floats = cJSON_GetNumberValue(f0) + cJSON_GetNumberValue(f1);
    cJSON_AddNumberToObject(stats, "sum_floats", sum_floats);
    char *out = cJSON_PrintUnformatted(root);
    void *prebuf_void = cJSON_malloc(512);
    memset(prebuf_void, 0, 512);
    char *prebuf = (char *)prebuf_void;
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, prebuf, 512, 0);
    (void)pre_ok;

    // step 4: Validate & Cleanup
    cJSON *retr_sum_d = cJSON_GetObjectItem(stats, "sum_doubles");
    double validated_sum_d = cJSON_GetNumberValue(retr_sum_d);
    (void)validated_sum_d;
    cJSON_DeleteItemFromObject(root, "floats");
    cJSON_free(out);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}