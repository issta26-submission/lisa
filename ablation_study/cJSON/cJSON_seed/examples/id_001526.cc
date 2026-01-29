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
//<ID> 1526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *info = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num_a = cJSON_CreateNumber(3.14);
    cJSON *num_b = cJSON_CreateNumber(2.71);
    cJSON *name = cJSON_CreateString("Alice");
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, num_b);
    cJSON_AddItemToObject(info, "name", name);
    cJSON_AddItemToObject(info, "active", flag);
    cJSON_AddItemToObject(root, "info", info);
    cJSON_AddItemToObject(root, "values", arr);

    // step 2: Configure
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, replacement);
    char *printed = cJSON_PrintUnformatted(root);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    cJSON *name_item = cJSON_GetObjectItem(info, "name");
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    cJSON_bool second_is_num = cJSON_IsNumber(second);
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    (void)replaced;
    (void)first_is_num;
    (void)second_is_num;
    (void)name_is_str;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}