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
//<ID> 1602
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "items", arr);
    const char *raw_json = "{\"raw_key\":123}";
    cJSON *raw = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToArray(arr, raw);
    cJSON_AddItemToArray(arr, cJSON_CreateString("hello"));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));

    // step 2: Configure
    cJSON_bool is_obj = cJSON_IsObject(data);
    cJSON_AddItemToObject(root, "data_is_object", cJSON_CreateBool(is_obj));
    int size = cJSON_GetArraySize(arr);
    cJSON_AddItemToObject(root, "items_count", cJSON_CreateNumber((double)size));
    cJSON_bool has_items = cJSON_HasObjectItem(data, "items");
    cJSON_AddItemToObject(root, "has_items_flag", cJSON_CreateNumber((double)has_items));

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON_bool cmp = cJSON_Compare(root, dup, 1);
    cJSON_AddItemToObject(dup, "comparison", cJSON_CreateNumber((double)cmp));

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}