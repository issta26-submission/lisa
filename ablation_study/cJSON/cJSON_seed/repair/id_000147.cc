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
//<ID> 147
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(flags, t);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "name", "fuzz_tester");

    // step 2: Configure
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON_AddItemToArray(values, n0);
    cJSON *n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(values, n1);
    cJSON *n2 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate and Validate
    cJSON *v0 = cJSON_GetArrayItem(values, 0);
    cJSON *v1 = cJSON_GetArrayItem(values, 1);
    cJSON *v2 = cJSON_GetArrayItem(values, 2);
    double sum = cJSON_GetNumberValue(v0) + cJSON_GetNumberValue(v1) + cJSON_GetNumberValue(v2);
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}