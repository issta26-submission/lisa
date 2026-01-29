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
//<ID> 1187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *arr = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(1.0);
    cJSON *num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *ref_item = cJSON_CreateString("referenced_string");

    // step 2: Configure
    cJSON_AddItemReferenceToArray(arr, ref_item);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *old_value = cJSON_CreateString("old_value");
    cJSON_AddItemToObject(child_obj, "key", old_value);
    cJSON_AddItemToObject(root, "child", child_obj);

    // step 3: Operate
    cJSON_bool is_arr = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "numbers_is_array", is_arr);
    cJSON *replacement = cJSON_CreateString("new_value");
    cJSON_ReplaceItemViaPointer(child_obj, old_value, replacement);
    cJSON_AddNullToObject(root, "nothing_here");

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}