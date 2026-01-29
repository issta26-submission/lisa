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
//<ID> 1472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"numbers\":[1,2],\"obj\":{\"a\":10,\"b\":20},\"name\":\"root\"}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *obj = cJSON_GetObjectItem(root, "obj");

    // step 2: Configure
    cJSON *newnum = cJSON_CreateNumber(99.9);
    cJSON_AddItemToArray(numbers, newnum);
    cJSON *created_array = cJSON_CreateArray();
    cJSON *n7 = cJSON_CreateNumber(7.0);
    cJSON *n8 = cJSON_CreateNumber(8.0);
    cJSON_AddItemToArray(created_array, n7);
    cJSON_AddItemToArray(created_array, n8);
    cJSON *obj_ref = cJSON_CreateObjectReference(obj);
    cJSON_AddItemReferenceToArray(created_array, obj_ref);
    cJSON *detached = cJSON_DetachItemFromObject(root, "obj");
    cJSON *detached_ref = cJSON_CreateObjectReference(detached);
    cJSON_AddItemReferenceToArray(created_array, detached_ref);
    cJSON_AddItemToObject(root, "added", created_array);
    cJSON_DeleteItemFromObject(root, "name");

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON_bool has_added = cJSON_HasObjectItem(root, "added");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON_Minify(buffer);
    cJSON_free(printed);
    cJSON_free(buffer);
    (void)first_is_number;
    (void)has_added;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}