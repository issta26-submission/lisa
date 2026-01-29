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
//<ID> 620
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[10,20,30],\"obj\":{\"key\":\"value\"}}";
    const char *parse_end1 = NULL;
    cJSON *root = cJSON_ParseWithOpts(json, &parse_end1, 1);
    cJSON *new_arr = cJSON_CreateArray();
    cJSON_AddItemToArray(new_arr, cJSON_CreateNumber(100.0));
    cJSON_AddItemToObject(root, "new_arr", new_arr);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    double v2 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 2));
    double total = v0 + v1 + v2;
    cJSON_AddItemToArray(new_arr, cJSON_CreateNumber(total));

    // step 3: Operate
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    int buflen = (int)sizeof(buffer);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, buflen, 1);
    const char *parse_end2 = NULL;
    cJSON *parsed_root = cJSON_ParseWithOpts(buffer, &parse_end2, 1);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(parsed_root, "obj");
    cJSON_bool parsed_arr_is_array = cJSON_IsArray(cJSON_GetObjectItem(parsed_root, "arr"));
    (void)arr_is_array;
    (void)printed;
    (void)parsed_arr_is_array;
    cJSON_Delete(parsed_root);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}