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
//<ID> 1601
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
    cJSON *raw_child = cJSON_CreateRaw("{\"inner\":42}");
    cJSON_AddItemToObject(root, "raw_child", raw_child);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.0));

    // step 2: Configure
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(nested, "flag", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "nested", nested);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "status", note);

    // step 3: Operate & Validate
    int array_size = cJSON_GetArraySize(arr);
    cJSON_AddItemToObject(root, "items_count", cJSON_CreateNumber((double)array_size));
    cJSON_bool nested_is_obj = cJSON_IsObject(nested);
    cJSON_AddItemToObject(root, "nested_is_object", cJSON_CreateNumber((double)nested_is_obj));
    cJSON_bool has_raw = cJSON_HasObjectItem(root, "raw_child");
    cJSON_AddItemToObject(root, "has_raw_child", cJSON_CreateNumber((double)has_raw));
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}