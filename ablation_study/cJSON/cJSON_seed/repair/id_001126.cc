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
//<ID> 1126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *meta = cJSON_CreateString("generated");
    cJSON_AddItemToObject(root, "note", meta);
    cJSON *result = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(root, "result", result);

    // step 3: Operate and Validate
    cJSON *values_obj = cJSON_GetObjectItemCaseSensitive(root, "values");
    cJSON *first = cJSON_GetArrayItem(values_obj, 0);
    double v1 = cJSON_GetNumberValue(first);
    cJSON *second = cJSON_GetArrayItem(values_obj, 1);
    double v2 = cJSON_GetNumberValue(second);
    double sum = v1 + v2;
    cJSON_SetNumberHelper(result, sum);
    char *out = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc(64);
    memset(scratch, 0, 64);
    scratch[0] = out[0];
    scratch[1] = out[1];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}