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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *metrics = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(metrics, "values", values);
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON *count = cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON *enabled = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON_DeleteItemFromArray(values, 1);
    cJSON_AddNumberToObject(metrics, "sum", 45.14);
    cJSON_AddNumberToObject(metrics, "avg", 22.57);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    int arr_size = cJSON_GetArraySize(values);
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled);
    double count_val = cJSON_GetNumberValue(count);
    int summary = arr_size + (int)count_val + (int)enabled_is_bool;
    void *tmp = cJSON_malloc(64);
    memset(tmp, 0, 64);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}