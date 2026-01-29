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
//<ID> 1181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const float fvals[] = {1.5f, 2.25f};
    const double dvals[] = {3.75, 4.125, 5.5};
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON *farr = cJSON_CreateFloatArray(fvals, 2);
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON_AddItemToObject(metrics, "floats", farr);
    cJSON_AddItemToObject(metrics, "doubles", darr);
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    cJSON *mixed = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(mixed, num1);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *f0 = cJSON_GetArrayItem(farr, 0);
    cJSON *d1 = cJSON_GetArrayItem(darr, 1);
    double sum = cJSON_GetNumberValue(f0) + cJSON_GetNumberValue(d1);
    cJSON_AddNumberToObject(root, "final", sum);
    cJSON *num2 = cJSON_CreateNumber(7.0);
    cJSON_AddItemToArray(mixed, num2);
    cJSON_AddItemToObject(root, "mixed", mixed);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}