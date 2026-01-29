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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *str_added = (cJSON *)0;
    cJSON *got_str = (cJSON *)0;
    cJSON *got_arr = (cJSON *)0;
    cJSON *got_raw = (cJSON *)0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_raw = (cJSON_bool)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    double flags_sum = 0.0;
    const char *retrieved_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    raw_item = cJSON_CreateRaw("{\"raw\":true}");
    str_added = cJSON_AddStringToObject(root, "inline", "inline_value");
    add_ok_array = cJSON_AddItemToObject(root, "array", arr);
    add_ok_raw = cJSON_AddItemToObject(root, "raw", raw_item);

    // step 3: Configure
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(7.0));
    is_arr = cJSON_IsArray(arr);
    is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "array_is_array", is_arr);
    cJSON_AddBoolToObject(root, "raw_is_raw", is_raw);

    // step 4: Operate
    got_str = cJSON_GetObjectItem(root, "inline");
    got_arr = cJSON_GetObjectItem(root, "array");
    got_raw = cJSON_GetObjectItem(root, "raw");
    retrieved_str = cJSON_GetStringValue(got_str);
    cJSON_AddStringToObject(root, "echo_inline", retrieved_str);

    // step 5: Validate
    flags_sum = (double)cJSON_IsArray(got_arr) + (double)cJSON_IsRaw(got_raw);
    cJSON_AddNumberToObject(root, "flags_sum", flags_sum);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}