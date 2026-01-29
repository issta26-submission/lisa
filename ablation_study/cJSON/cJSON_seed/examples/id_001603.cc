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
//<ID> 1603
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
    cJSON *raw_item = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str_item);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON_AddItemToObject(nested, "num", cJSON_CreateNumber(42.0));

    // step 2: Configure
    cJSON_bool has_items = cJSON_HasObjectItem(root, "items");
    cJSON_AddItemToObject(root, "has_items", cJSON_CreateNumber((double)has_items));
    cJSON_bool nested_is_obj = cJSON_IsObject(nested);
    cJSON_AddItemToObject(root, "nested_is_object", cJSON_CreateNumber((double)nested_is_obj));
    int items_count = cJSON_GetArraySize(arr);
    cJSON_AddItemToObject(root, "items_size", cJSON_CreateNumber((double)items_count));

    // step 3: Operate & Validate
    cJSON *nested_copy = cJSON_Duplicate(nested, 1);
    cJSON_AddItemToObject(root, "nested_copy", nested_copy);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}