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
//<ID> 698
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    const char *names[] = { "alpha", "beta", "gamma" };
    cJSON *arr = cJSON_CreateStringArray(names, 3);
    cJSON_AddItemToObject(root, "names", arr);
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", missing);
    cJSON_AddBoolToObject(root, "enabled", 1);

    // step 2: Configure
    cJSON *names_copy = cJSON_Duplicate(arr, 1);
    cJSON_AddItemToObject(root, "names_copy", names_copy);
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "arr_is_array", arr_is_array);
    cJSON_bool arr_is_raw = cJSON_IsRaw(arr);
    cJSON_AddBoolToObject(root, "arr_is_raw", arr_is_raw);

    // step 3: Operate & Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    const char *first_value = cJSON_GetStringValue(first_item);
    (void)first_value;
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}