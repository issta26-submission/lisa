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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const words[3] = { "one", "two", "three" };
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *str_array = cJSON_CreateStringArray(words, 3);
    cJSON *solo_str = cJSON_CreateString("solo");
    cJSON_AddItemToArray(arr, str_array);
    cJSON_AddItemToArray(arr, solo_str);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "maybe", maybe_null);
    int buf_len = 256;
    char *print_buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(print_buf, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, print_buf, buf_len, 1);

    // step 3: Operate & Validate
    int total_items = cJSON_GetArraySize(arr);
    cJSON *first_child = cJSON_GetArrayItem(arr, 0); /* this is the string array created above */
    cJSON *second_word = cJSON_GetArrayItem(first_child, 1); /* "two" */
    const char *picked_value = cJSON_GetStringValue(second_word);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON_bool null_detected = cJSON_IsNull(maybe_null);
    double summary_val = (double)total_items + (double)root_is_obj + (double)null_detected + (double)printed_ok;
    cJSON *summary = cJSON_CreateNumber(summary_val);
    cJSON_AddItemToObject(root, "summary", summary);
    cJSON *picked = cJSON_CreateString(picked_value);
    cJSON_AddItemToObject(root, "picked", picked);

    // step 4: Cleanup
    cJSON_free(print_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}