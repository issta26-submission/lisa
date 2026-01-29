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
//<ID> 692
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *strings_arr[] = { "one", "two", "three" };
    cJSON *str_array = cJSON_CreateStringArray(strings_arr, 3);
    cJSON_AddItemToObject(root, "strings", str_array);
    cJSON *raw_item = cJSON_CreateRaw("{\"raw\":1}");
    cJSON_AddItemToObject(root, "raw", raw_item);

    // step 2: Configure
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    cJSON *bool_item = cJSON_AddBoolToObject(root, "active", 1);

    // step 3: Operate
    cJSON *got_array = cJSON_GetObjectItem(root, "strings");
    cJSON_bool array_check = cJSON_IsArray(got_array);
    int arr_size = cJSON_GetArraySize(got_array);
    cJSON *first_elem = cJSON_GetArrayItem(got_array, 0);
    const char *first_str = cJSON_GetStringValue(first_elem);
    (void)first_str;
    cJSON *got_raw = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_check = cJSON_IsRaw(got_raw);
    (void)array_check; (void)arr_size; (void)raw_check; (void)bool_item;

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}