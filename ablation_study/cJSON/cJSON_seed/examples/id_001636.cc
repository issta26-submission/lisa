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
//<ID> 1636
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON *s1_ref_array = cJSON_CreateArrayReference(s1);
    cJSON_AddItemToObject(root, "s1_ref_array", s1_ref_array);

    // step 2: Configure
    char *s1_val = cJSON_GetStringValue(s1);
    cJSON *s1_copy = cJSON_CreateString(s1_val);
    cJSON_AddItemToObject(root, "s1_copy", s1_copy);
    cJSON_bool s1_is_str = cJSON_IsString(s1);
    cJSON *s1_is_string_flag = cJSON_CreateBool(s1_is_str);
    cJSON_AddItemToObject(root, "s1_is_string", s1_is_string_flag);

    // step 3: Operate & Validate
    cJSON *arr_dup = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "arr_dup", arr_dup);
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    char *second_str = cJSON_GetStringValue(second_item);
    cJSON *second_copy = cJSON_CreateString(second_str);
    cJSON_AddItemToObject(root, "second_copy", second_copy);

    // step 4: Cleanup
    char *unfmt = cJSON_PrintUnformatted(root);
    cJSON_free(unfmt);
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}