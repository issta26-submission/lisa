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
//<ID> 1606
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
    cJSON *str1 = cJSON_CreateString("first");
    cJSON *str2 = cJSON_CreateString("second");
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":123}");
    cJSON_AddItemToObject(meta, "payload", raw);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    int items_count = cJSON_GetArraySize(arr);
    cJSON *items_count_num = cJSON_CreateNumber((double)items_count);
    cJSON_AddItemToObject(root, "items_count", items_count_num);
    cJSON_bool has_meta = cJSON_HasObjectItem(root, "meta");
    cJSON *has_meta_num = cJSON_CreateNumber((double)has_meta);
    cJSON_AddItemToObject(root, "has_meta", has_meta_num);
    cJSON_bool meta_is_object = cJSON_IsObject(meta);
    cJSON *meta_is_object_num = cJSON_CreateNumber((double)meta_is_object);
    cJSON_AddItemToObject(root, "meta_is_object", meta_is_object_num);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}