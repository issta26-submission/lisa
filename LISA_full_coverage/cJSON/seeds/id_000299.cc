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
//<ID> 299
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *num1 = cJSON_CreateNumber(10.0);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, flag_false);

    // step 2: Configure
    cJSON *replacement_num = cJSON_CreateNumber(20.5);
    cJSON_bool replace_ok = cJSON_ReplaceItemInArray(arr, 0, replacement_num);
    (void)replace_ok;
    cJSON *raw = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":1,\"y\":2}");
    (void)raw;

    // step 3: Operate and Validate
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_false = cJSON_IsFalse(second_item);
    cJSON_AddBoolToObject(root, "root_is_object", root_is_obj);
    cJSON_AddBoolToObject(root, "second_is_false", second_is_false);
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}