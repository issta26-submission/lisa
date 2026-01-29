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
//<ID> 1465
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"list\":[10,20],\"obj\":{\"key\":\"value\"}}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *list = cJSON_GetObjectItem(root, "list");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");
    cJSON *first_item = cJSON_GetArrayItem(list, 0);
    cJSON_bool first_invalid = cJSON_IsInvalid(first_item);
    (void)first_invalid;

    // step 2: Configure
    cJSON_AddItemReferenceToArray(list, first_item);
    cJSON *replacement_str = cJSON_CreateString("newvalue");
    cJSON_ReplaceItemInObject(obj, "key", replacement_str);
    cJSON *replacement_array = cJSON_CreateArray();
    cJSON_AddItemToArray(replacement_array, cJSON_CreateNumber(7.0));
    cJSON_AddItemToArray(replacement_array, cJSON_CreateNumber(8.0));
    cJSON_ReplaceItemViaPointer(root, list, replacement_array);

    // step 3: Operate & Validate
    cJSON *new_list = cJSON_GetObjectItem(root, "list");
    int new_list_size = cJSON_GetArraySize(new_list);
    cJSON *obj_key_item = cJSON_GetObjectItem(obj, "key");
    cJSON_bool key_invalid = cJSON_IsInvalid(obj_key_item);
    (void)new_list_size;
    (void)key_invalid;
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