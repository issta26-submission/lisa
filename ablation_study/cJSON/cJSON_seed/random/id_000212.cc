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
//<ID> 212
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
    cJSON *str_ver = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    char *json_text = (char *)0;
    const char *version = (const char *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Configure
    version = cJSON_Version();
    str_ver = cJSON_CreateString(version);
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("example");

    // step 4: Operate
    add_ok1 = cJSON_AddItemToArray(arr, str_ver);
    add_ok2 = cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddTrueToObject(root, "active");
    is_array = cJSON_IsArray(arr);
    cJSON_AddBoolToObject(root, "is_array", is_array);

    // step 5: Validate
    int size = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", count_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}