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
//<ID> 1604
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
    const char *vals[2] = { "one", "two" };
    cJSON_AddItemToArray(arr, cJSON_CreateString(vals[0]));
    cJSON_AddItemToArray(arr, cJSON_CreateString(vals[1]));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *raw = cJSON_CreateRaw("{\"embedded\":true}");
    cJSON_AddItemToObject(root, "meta", raw);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *size_num = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "items_count", size_num);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_num = cJSON_CreateNumber((double)has_meta);
    cJSON_AddItemToObject(root, "has_meta", has_meta_num);

    // step 3: Operate & Validate
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(meta_item);
    cJSON *meta_is_obj_num = cJSON_CreateNumber((double)meta_is_obj);
    cJSON_AddItemToObject(root, "meta_is_object", meta_is_obj_num);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}