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
//<ID> 1607
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
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.5));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(-3.25));
    const char *raw_json = "{\"nested\":true,\"items\":3}";
    cJSON *raw_item = cJSON_CreateRaw(raw_json);
    cJSON_AddItemToObject(root, "raw", raw_item);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    int values_count = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)values_count);
    cJSON_AddItemToObject(meta, "values_count", count_num);
    cJSON_bool has_values = cJSON_HasObjectItem(root, "values");
    cJSON *has_values_num = cJSON_CreateNumber((double)has_values);
    cJSON_AddItemToObject(meta, "has_values", has_values_num);
    cJSON_bool meta_is_obj = cJSON_IsObject(meta);
    cJSON *meta_is_obj_num = cJSON_CreateNumber((double)meta_is_obj);
    cJSON_AddItemToObject(root, "meta_is_object", meta_is_obj_num);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}