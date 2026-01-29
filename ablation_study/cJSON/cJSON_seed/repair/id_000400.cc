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
//<ID> 400
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "owner", "tester");
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *values_from_root = cJSON_GetObjectItem(root, "values");
    int size_before = cJSON_GetArraySize(values_from_root);
    cJSON *first_item = cJSON_GetArrayItem(values_from_root, 0);
    cJSON *dup_item = cJSON_Duplicate(first_item, 1);
    cJSON_AddItemToArray(values_from_root, dup_item);

    // step 3: Operate
    int size_after = cJSON_GetArraySize(values_from_root);
    cJSON *last_item = cJSON_GetArrayItem(values_from_root, size_after - 1);
    double v0 = cJSON_GetNumberValue(first_item);
    double vlast = cJSON_GetNumberValue(last_item);
    double total = v0 + vlast;
    cJSON_AddNumberToObject(root, "total", total);

    // step 4: Validate and Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}