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
//<ID> 219
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
    cJSON *item_str = (cJSON *)0;
    cJSON *item_num = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool is_array_check = (cJSON_bool)0;
    char *json_text = (char *)0;
    const char *version_text = (const char *)0;
    int array_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Configure
    item_str = cJSON_CreateString("element_one");
    item_num = cJSON_CreateNumber(2.0);
    cJSON_AddTrueToObject(root, "active_flag");

    // step 4: Operate
    add_ok1 = cJSON_AddItemToArray(arr, item_str);
    add_ok2 = cJSON_AddItemToArray(arr, item_num);
    is_array_check = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "is_array", is_array_check);
    version_text = cJSON_Version();
    cJSON_AddStringToObject(root, "cjson_version", version_text);

    // step 5: Validate
    array_size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)array_size);
    cJSON_AddItemToObject(root, "count", count_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}