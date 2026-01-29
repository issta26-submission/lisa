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
//<ID> 230
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
    const char *raw_text = "{\"embedded\":true}";
    cJSON *raw = cJSON_CreateRaw(raw_text);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON *str1 = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "raw_payload", raw);

    // step 3: Operate & Validate
    cJSON *retrieved_values = cJSON_GetObjectItem(root, "values");
    cJSON *dup_values = cJSON_Duplicate(retrieved_values, 1);
    int dup_size = cJSON_GetArraySize(dup_values);
    cJSON *dup_size_num = cJSON_CreateNumber((double)dup_size);
    cJSON_AddItemToObject(root, "dup_size", dup_size_num);
    cJSON *retrieved_raw = cJSON_GetObjectItem(root, "raw_payload");
    cJSON_bool raw_flag = cJSON_IsRaw(retrieved_raw);
    cJSON *raw_flag_num = cJSON_CreateNumber((double)raw_flag);
    cJSON_AddItemToObject(root, "is_raw", raw_flag_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(dup_values);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}