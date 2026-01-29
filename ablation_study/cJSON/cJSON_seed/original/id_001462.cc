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
//<ID> 1462
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[1,2],\"obj\":{\"key\":\"value\"}}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *key_item = cJSON_GetObjectItem(obj, "key");
    cJSON_bool key_invalid = cJSON_IsInvalid(key_item);

    // step 2: Configure
    cJSON *dup_key = cJSON_Duplicate(key_item, 1);
    cJSON_bool added_ref = cJSON_AddItemReferenceToArray(arr, dup_key);
    cJSON *new_str = cJSON_CreateString("newval");
    cJSON_bool replaced_in_obj = cJSON_ReplaceItemInObject(obj, "key", new_str);
    cJSON *new_obj = cJSON_CreateObject();
    cJSON_AddNumberToObject(new_obj, "replaced", 99.0);
    cJSON_bool replaced_via_ptr = cJSON_ReplaceItemViaPointer(root, obj, new_obj);

    // step 3: Operate & Validate
    cJSON *third = cJSON_GetArrayItem(arr, 2);
    cJSON_bool third_invalid = cJSON_IsInvalid(third);
    const char *third_str = cJSON_GetStringValue(third);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    if (printed && buffer) memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    (void)key_invalid;
    (void)added_ref;
    (void)replaced_in_obj;
    (void)replaced_via_ptr;
    (void)third_invalid;
    (void)third_str;
    (void)first_val;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}