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
//<ID> 233
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
    cJSON *meta = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("{\"flag\":\"raw\"}");

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, num1);
    cJSON_AddItemToArray(items, str1);
    cJSON_AddItemToArray(items, num2);
    cJSON_AddItemToArray(items, raw);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "items");
    int size_original = cJSON_GetArraySize(items);
    int size_via_get = cJSON_GetArraySize(retrieved);
    cJSON *size_num = cJSON_CreateNumber((double)size_via_get);
    cJSON_AddItemToObject(meta, "size", size_num);
    cJSON *dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", dup);
    cJSON_bool raw_flag = cJSON_IsRaw(raw);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)raw_flag);
    cJSON_AddItemToObject(meta, "raw_flag", raw_flag_num);
    int size_dup = cJSON_GetArraySize(dup);
    cJSON *dup_size_num = cJSON_CreateNumber((double)size_dup);
    cJSON_AddItemToObject(meta, "copy_size", dup_size_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}