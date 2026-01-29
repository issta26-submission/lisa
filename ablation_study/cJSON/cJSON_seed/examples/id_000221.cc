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
//<ID> 221
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
    cJSON *first = cJSON_CreateString("first_value");
    cJSON_AddItemToArray(arr, first);
    cJSON_AddItemToObject(root, "my_array", arr);

    // step 2: Configure
    char * dyn = (char *)cJSON_malloc((size_t)32);
    memset(dyn, 0, (size_t)32);
    memcpy(dyn, "dynamic_value_01", (size_t)16);
    cJSON *dyn_item = cJSON_CreateString(dyn);
    cJSON_AddItemToObject(root, "dynamic", dyn_item);
    cJSON *child_obj = cJSON_CreateObject();
    cJSON *child_val = cJSON_CreateString("child_string");
    cJSON_AddItemToObject(child_obj, "c_key", child_val);
    cJSON_AddItemToObject(root, "child", child_obj);

    // step 3: Operate & Validate
    cJSON_bool has_array = cJSON_HasObjectItem(root, "my_array");
    cJSON *has_flag = cJSON_CreateNumber((double)has_array);
    cJSON_AddItemToObject(root, "has_my_array", has_flag);
    cJSON *added_num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, added_num);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(dyn);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}