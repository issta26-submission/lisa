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
//<ID> 1093
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,null],\"name\":\"old\",\"num\":5}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_check = cJSON_IsArray(arr);
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON_bool is_third_null = cJSON_IsNull(third);
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double num_val = cJSON_GetNumberValue(num_item);
    cJSON *replacement_third = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(arr, third, replacement_third);
    cJSON *new_name = cJSON_CreateString("new_name");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);
    double new_num_val = num_val + (double)arr_check + (double)is_third_null + 1.0;
    cJSON *new_num = cJSON_CreateNumber(new_num_val);
    cJSON_ReplaceItemViaPointer(root, num_item, new_num);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}