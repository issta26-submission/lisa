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
//<ID> 725
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *false_item = cJSON_CreateFalse();
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON *temp_str = cJSON_CreateString("transient");

    // step 2: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "enabled", false_item);
    cJSON_AddItemToObject(root, "doubles", dbl_arr);
    cJSON_AddItemToObject(root, "temp", temp_str);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_items = cJSON_GetObjectItemCaseSensitive(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *parsed_doubles = cJSON_GetObjectItemCaseSensitive(parsed, "doubles");
    cJSON *second_num_item = cJSON_GetArrayItem(parsed_doubles, 1);
    double second_val = cJSON_GetNumberValue(second_num_item);
    (void)first_str;
    (void)second_val;
    (void)version;

    // step 4: Cleanup
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "temp");
    cJSON_AddItemToArray(arr, detached);
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}