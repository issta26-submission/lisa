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
//<ID> 622
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
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddStringToObject(root, "label", "samples");

    // step 2: Configure
    cJSON *array_ref = cJSON_GetObjectItem(root, "array");
    cJSON_bool is_array = cJSON_IsArray(array_ref);
    cJSON *first = cJSON_GetArrayItem(array_ref, 0);
    cJSON *second = cJSON_GetArrayItem(array_ref, 1);
    double v1 = cJSON_GetNumberValue(first);
    double v2 = cJSON_GetNumberValue(second);
    cJSON *sum_item = cJSON_CreateNumber(v1 + v2);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 3: Operate
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), (cJSON_bool)0);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(buffer, &parse_end, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "array");
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}