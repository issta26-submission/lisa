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
//<ID> 2413
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
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON *str = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddStringToObject(root, "label", "root_label");

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    char *buffer = (char *)cJSON_malloc((size_t)64);
    memset(buffer, 0, (size_t)64);

    // step 3: Operate
    cJSON *arr_item0 = cJSON_GetArrayItem(arr, 0);
    double nval = cJSON_GetNumberValue(arr_item0);
    cJSON *arr_item1 = cJSON_GetArrayItem(arr, 1);
    char *sval = cJSON_GetStringValue(arr_item1);
    buffer[0] = (char)(int)nval;
    buffer[1] = sval[0];
    cJSON *dup_arr = cJSON_GetObjectItem(dup, "data");
    cJSON *dup_arr_item0 = cJSON_GetArrayItem(dup_arr, 0);
    double dup_nval = cJSON_GetNumberValue(dup_arr_item0);
    buffer[2] = (char)(int)dup_nval;

    // step 4: Validate and Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}