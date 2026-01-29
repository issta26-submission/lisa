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
//<ID> 1552
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
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.5));
    cJSON_AddNumberToObject(root, "scale", 2.0);

    // step 2: Configure
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "values_copy", arr_dup);
    char *printed = cJSON_Print(root);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double val = cJSON_GetNumberValue(second);
    cJSON *scale_item = cJSON_GetObjectItem(root, "scale");
    double scale_val = cJSON_GetNumberValue(scale_item);
    double scaled = val * scale_val;
    cJSON_AddNumberToObject(root, "scaled_second", scaled);
    int dup_size = cJSON_GetArraySize(arr_dup);
    double dup_second = cJSON_GetNumberValue(cJSON_GetArrayItem(arr_dup, 1));
    cJSON_AddNumberToObject(root, "dup_diff", dup_second - val);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}