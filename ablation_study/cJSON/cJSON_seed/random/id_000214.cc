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
//<ID> 214
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
    cJSON *true_item = (cJSON *)0;
    cJSON *ver_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *size_item = (cJSON *)0;
    char *json_text = (char *)0;
    const char *ver = (const char *)0;
    cJSON_bool is_arr = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    int size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "data", arr);

    // step 3: Configure
    true_item = cJSON_AddTrueToObject(root, "feature_enabled");
    ver = cJSON_Version();
    ver_item = cJSON_AddStringToObject(root, "version", ver);
    is_arr = cJSON_IsArray(arr);
    count_item = cJSON_CreateNumber((double)is_arr);
    cJSON_AddItemToObject(root, "is_array_flag", count_item);

    // step 4: Operate
    str_item = cJSON_CreateString("element_one");
    num_item = cJSON_CreateNumber(42.0);
    add_ok1 = cJSON_AddItemToArray(arr, str_item);
    add_ok2 = cJSON_AddItemToArray(arr, num_item);

    // step 5: Validate
    size = cJSON_GetArraySize(arr);
    size_item = cJSON_CreateNumber((double)size);
    cJSON_AddItemToObject(root, "count", size_item);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}