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
//<ID> 1186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double dvals[3] = {1.5, 2.5, 3.0};
    float fvals[2] = {4.0f, 5.5f};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON *farr = cJSON_CreateFloatArray(fvals, 2);
    cJSON_AddItemToObject(root, "doubles", darr);
    cJSON_AddItemToObject(root, "floats", farr);

    // step 2: Configure
    cJSON *extra_d = cJSON_CreateNumber(6.25);
    cJSON_AddItemToArray(darr, extra_d);
    cJSON *extra_f = cJSON_CreateNumber(7.75);
    cJSON_AddItemToArray(farr, extra_f);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    double d0 = cJSON_GetNumberValue(cJSON_GetArrayItem(darr, 0));
    double d3 = cJSON_GetNumberValue(cJSON_GetArrayItem(darr, 3));
    double f0 = cJSON_GetNumberValue(cJSON_GetArrayItem(farr, 0));
    double total = d0 + d3 + f0;
    cJSON_AddNumberToObject(root, "total", total);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}