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
//<ID> 1609
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
    cJSON *s1 = cJSON_CreateString("apple");
    cJSON *s2 = cJSON_CreateString("banana");
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "raw_payload", raw);

    // step 2: Configure
    int size = cJSON_GetArraySize(items);
    cJSON *size_num = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "items_count", size_num);
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON *has_items_num = cJSON_CreateNumber((double)has_items);
    cJSON_AddItemToObject(root, "has_items_flag", has_items_num);
    cJSON_bool raw_is_object = cJSON_IsObject(raw);
    cJSON *raw_is_object_num = cJSON_CreateNumber((double)raw_is_object);
    cJSON_AddItemToObject(root, "raw_is_object", raw_is_object_num);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_val = cJSON_GetStringValue(first);
    cJSON *first_copy = cJSON_CreateString(first_val);
    cJSON_AddItemToObject(root, "first_item_copy", first_copy);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    char *full = cJSON_Print(root);
    cJSON_free(full);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}