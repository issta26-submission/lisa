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
//<ID> 696
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *words[] = {"alpha", "beta", "gamma"};
    cJSON *str_arr = cJSON_CreateStringArray(words, 3);
    cJSON_AddItemToObject(root, "words", str_arr);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON_bool is_array = cJSON_IsArray(str_arr);
    cJSON_bool is_raw = cJSON_IsRaw(maybe_null);
    int count = cJSON_GetArraySize(str_arr);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "words_count", count_num);
    cJSON *is_array_num = cJSON_CreateNumber((double)is_array);
    cJSON_AddItemToObject(root, "words_is_array", is_array_num);
    cJSON *is_raw_num = cJSON_CreateNumber((double)is_raw);
    cJSON_AddItemToObject(root, "maybe_null_is_raw", is_raw_num);

    // step 3: Operate
    cJSON *words_copy = cJSON_Duplicate(str_arr, 1);
    cJSON_AddItemToObject(root, "words_copy", words_copy);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}