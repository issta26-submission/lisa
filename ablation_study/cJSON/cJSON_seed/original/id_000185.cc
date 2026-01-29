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
//<ID> 185
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
    cJSON *num_item = cJSON_CreateNumber(123.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "array", arr);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);

    // step 2: Configure
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    size_t prebuf_size = 128;
    char *prebuf = (char *)cJSON_malloc(prebuf_size);
    memset(prebuf, 0, prebuf_size);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    size_t printed_len = strlen(printed);
    size_t to_copy = printed_len < (prebuf_size - 1) ? printed_len : (prebuf_size - 1);
    memcpy(prebuf, printed, to_copy);
    prebuf[to_copy] = '\0';
    cJSON_Minify(prebuf);
    char *full_copy = (char *)cJSON_malloc(printed_len + 1);
    memset(full_copy, 0, printed_len + 1);
    memcpy(full_copy, printed, printed_len + 1);
    cJSON_Minify(full_copy);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_bool is_null = cJSON_IsNull(null_item);
    const char *second_str = cJSON_GetStringValue(cJSON_GetArrayItem(arr, 1));
    int summary = arr_size + (int)is_null + (int)first_val + (second_str ? (int)second_str[0] : 0) + (root ? root->type : 0);
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(full_copy);
    cJSON_free(prebuf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}