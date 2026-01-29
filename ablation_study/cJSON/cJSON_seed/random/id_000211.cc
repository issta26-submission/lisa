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
//<ID> 211
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
    cJSON *version_item = (cJSON *)0;
    cJSON *is_array_flag_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    const char *version_text = (const char *)0;
    cJSON_bool is_array_val = (cJSON_bool)0;
    char *json_text = (char *)0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    version_text = cJSON_Version();
    version_item = cJSON_CreateString(version_text);
    true_item = cJSON_AddTrueToObject(root, "always_true");

    // step 4: Operate
    cJSON_AddItemToArray(arr, version_item);
    is_array_val = cJSON_IsArray(arr);
    is_array_flag_item = cJSON_CreateBool(is_array_val);
    cJSON_AddItemToArray(arr, is_array_flag_item);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}