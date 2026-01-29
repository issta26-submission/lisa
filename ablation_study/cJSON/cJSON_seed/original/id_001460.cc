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
//<ID> 1460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(items, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));

    // step 2: Configure
    cJSON *obj_ref = cJSON_CreateObject();
    cJSON_AddItemToObject(obj_ref, "val", cJSON_CreateNumber(99.0));
    cJSON_AddItemReferenceToArray(items, obj_ref);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_item);
    (void)first_is_invalid;

    // step 3: Operate & Validate
    cJSON_AddItemToObject(root, "replace_me", cJSON_CreateString("old_value"));
    cJSON *replacement_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(replacement_obj, "new_flag", cJSON_CreateBool(1));
    cJSON_ReplaceItemInObject(root, "replace_me", replacement_obj);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON *replacement_num = cJSON_CreateNumber(777.0);
    cJSON_ReplaceItemViaPointer(items, second_item, replacement_num);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}