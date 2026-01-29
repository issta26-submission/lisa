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
//<ID> 691
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *items[] = { "one", "two", "three" };
    cJSON *root = cJSON_CreateObject();
    cJSON *str_arr = cJSON_CreateStringArray(items, 3);
    cJSON_AddItemToObject(root, "strings", str_arr);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "empty", null_item);

    // step 2: Configure
    cJSON_bool arr_is_array = cJSON_IsArray(str_arr);
    cJSON_bool arr_is_raw = cJSON_IsRaw(str_arr);
    cJSON_AddBoolToObject(root, "strings_is_array", arr_is_array);
    cJSON_AddBoolToObject(root, "strings_is_raw", arr_is_raw);

    // step 3: Operate
    cJSON *arr_copy = cJSON_Duplicate(str_arr, 1);
    cJSON_AddItemToObject(root, "strings_copy", arr_copy);
    char *out_unformatted = cJSON_PrintUnformatted(root);
    char *out_formatted = cJSON_Print(root);
    cJSON_free(out_unformatted);
    cJSON_free(out_formatted);

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}