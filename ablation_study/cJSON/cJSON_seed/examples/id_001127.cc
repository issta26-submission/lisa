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
//<ID> 1127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(3.0);
    cJSON *n1 = cJSON_CreateNumber(4.0);
    const char *ver = cJSON_Version();

    // step 2: Configure
    cJSON_AddItemToArray(values, n0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *ver_ref = cJSON_CreateStringReference(ver);
    cJSON_AddItemToObject(root, "version", ver_ref);
    cJSON *label_ref = cJSON_CreateStringReference("example");
    cJSON_AddItemToObject(root, "label", label_ref);

    // step 3: Operate & Validate
    cJSON *arr_ref = cJSON_CreateArrayReference(values);
    cJSON_AddItemToObject(root, "values_ref", arr_ref);
    cJSON *it0 = cJSON_GetArrayItem(values, 0);
    cJSON *it1 = cJSON_GetArrayItem(values, 1);
    double v0 = cJSON_GetNumberValue(it0);
    double v1 = cJSON_GetNumberValue(it1);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *replacement = cJSON_CreateStringReference("replaced_ref");
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values_ref", replacement);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}