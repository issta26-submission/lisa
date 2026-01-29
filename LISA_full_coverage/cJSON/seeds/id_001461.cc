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
//<ID> 1461
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
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);

    // step 2: Configure
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(refs, obj);
    cJSON_bool first_was_invalid = cJSON_IsInvalid(first_item);
    cJSON *new_str = cJSON_CreateString("replaced");
    cJSON_bool replace_obj_ok = cJSON_ReplaceItemInObject(obj, "key", new_str);
    cJSON *num99 = cJSON_CreateNumber(99.9);
    cJSON_bool replace_via_ptr_ok = cJSON_ReplaceItemViaPointer(arr, first_item, num99);

    // step 3: Operate & Validate
    cJSON *first_after = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first_after);
    (void)add_ref_ok;
    (void)first_was_invalid;
    (void)replace_obj_ok;
    (void)replace_via_ptr_ok;
    (void)first_val;
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}