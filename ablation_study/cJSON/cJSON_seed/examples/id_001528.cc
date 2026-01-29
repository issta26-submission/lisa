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
//<ID> 1528
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
    cJSON *num_item = cJSON_CreateNumber(2.718);
    cJSON *str_item = cJSON_CreateString("greeting");
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);

    // step 2: Configure
    cJSON *replacement_num = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 0, replacement_num);
    cJSON *flag = cJSON_CreateBool(1);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON *second_elem = cJSON_GetArrayItem(arr, 1);
    cJSON_bool first_is_num = cJSON_IsNumber(first_elem);
    cJSON_bool second_is_str = cJSON_IsString(second_elem);
    (void)first_is_num;
    (void)second_is_str;

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal_structs = cJSON_Compare(root, duplicate, 1);
    (void)equal_structs;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(duplicate);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}