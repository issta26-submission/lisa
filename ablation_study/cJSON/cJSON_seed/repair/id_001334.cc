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
//<ID> 1334
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "values", values);
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON *n3 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToArray(values, n3);

    // step 2: Configure
    int size_before = cJSON_GetArraySize(values);
    cJSON *target = cJSON_GetArrayItem(values, 1);
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemViaPointer(values, target, replacement);

    // step 3: Operate and Validate
    int size_after = cJSON_GetArraySize(values);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    double val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    double val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 1));
    ((char *)scratch)[0] = out ? out[0] : 0;
    ((char *)scratch)[1] = (char)('0' + (size_before == size_after ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (replaced ? 1 : 0));
    ((char *)scratch)[3] = (char)('0' + ((int)val0 % 10));
    ((char *)scratch)[4] = (char)('0' + ((int)val1 % 10));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}