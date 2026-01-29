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
//<ID> 1470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"node\":{\"value\":100},\"arr\":[1,2],\"meta\":\"ok\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *node = cJSON_GetObjectItem(root, "node");
    cJSON *value = cJSON_GetObjectItem(node, "value");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");

    // step 2: Configure
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(child, "inner", cJSON_CreateNumber(7.5));
    cJSON_AddItemToObject(root, "newobj", child);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_bool add_ref_ok = cJSON_AddItemReferenceToArray(arr, child_ref);
    cJSON *detached_node = cJSON_DetachItemFromObject(root, "node");
    cJSON_AddItemToArray(arr, detached_node);

    // step 3: Operate & Validate
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_elem);
    cJSON_bool value_is_number = cJSON_IsNumber(value);
    cJSON_bool has_newobj = cJSON_HasObjectItem(root, "newobj");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    (void)first_is_number;
    (void)value_is_number;
    (void)has_newobj;
    (void)add_ref_ok;

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(root, "arr");
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}