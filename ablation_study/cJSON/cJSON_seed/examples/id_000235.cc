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
//<ID> 235
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
    cJSON *obj = cJSON_CreateObject();
    cJSON *raw = cJSON_CreateRaw("raw_payload");
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.14);
    cJSON *str = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, str);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", obj);
    cJSON_AddItemToObject(root, "raw", raw);

    // step 3: Operate & Validate
    cJSON *items = cJSON_GetObjectItem(root, "items");
    int items_count = cJSON_GetArraySize(items);
    cJSON *items_count_num = cJSON_CreateNumber((double)items_count);
    cJSON_AddItemToObject(root, "items_count", items_count_num);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)raw_flag);
    cJSON_AddItemToObject(root, "raw_is_raw", raw_flag_num);
    cJSON *dup_arr = cJSON_Duplicate(items, 1);
    int dup_count = cJSON_GetArraySize(dup_arr);
    cJSON *dup_count_num = cJSON_CreateNumber((double)dup_count);
    cJSON_AddItemToObject(root, "dup_count", dup_count_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup_arr);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}