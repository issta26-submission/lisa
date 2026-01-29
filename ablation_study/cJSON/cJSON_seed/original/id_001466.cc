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
//<ID> 1466
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"arr\":[1,2],\"obj\":{\"key\":\"val\"},\"name\":\"orig\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first);

    // step 2: Configure
    cJSON *new_str = cJSON_CreateString("replaced");
    cJSON_bool replaced_in_obj = cJSON_ReplaceItemInObject(obj, "key", new_str);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *new_num = cJSON_CreateNumber(99.0);
    cJSON_bool replaced_in_arr = cJSON_ReplaceItemViaPointer(arr, second, new_num);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj);
    cJSON_bool added_ref = cJSON_AddItemReferenceToArray(arr, obj_ref);

    // step 3: Operate & Validate
    cJSON *ref_item = cJSON_GetArrayItem(arr, 2);
    cJSON_bool ref_is_invalid = cJSON_IsInvalid(ref_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    (void)first_invalid;
    (void)replaced_in_obj;
    (void)replaced_in_arr;
    (void)added_ref;
    (void)ref_is_invalid;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}