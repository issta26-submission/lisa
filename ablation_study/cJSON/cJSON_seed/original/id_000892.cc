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
//<ID> 892
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    double dvals[3] = {1.1, 2.2, 3.3};
    float fvals[2] = {4.4f, 5.5f};
    cJSON *darr = cJSON_CreateDoubleArray(dvals, 3);
    cJSON *farr = cJSON_CreateFloatArray(fvals, 2);
    cJSON_AddItemToObject(root, "doubles", darr);
    cJSON_AddItemToObject(root, "floats", farr);

    // step 2: Configure
    cJSON_AddNumberToObject(meta, "count_doubles", (double)cJSON_GetArraySize(darr));
    cJSON_AddNumberToObject(meta, "count_floats", (double)cJSON_GetArraySize(farr));
    cJSON_AddStringToObject(meta, "description", "sample arrays");

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    cJSON *second_double = cJSON_GetArrayItem(darr, 1);
    double val_double = cJSON_GetNumberValue(second_double);
    cJSON *first_float = cJSON_GetArrayItem(farr, 0);
    double val_float = cJSON_GetNumberValue(first_float);
    double combined = val_double + val_float;
    cJSON_AddNumberToObject(meta, "combined_sample", combined);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "floats");
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}