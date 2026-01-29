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
//<ID> 1524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num_item = cJSON_CreateNumber(10.0);
    cJSON *str_item = cJSON_CreateString("hello");
    cJSON_bool added_num = cJSON_AddItemToArray(array, num_item);
    cJSON_bool added_str = cJSON_AddItemToArray(array, str_item);
    cJSON_bool added_array = cJSON_AddItemToObject(root, "items", array);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON_bool second_is_string = cJSON_IsString(second);
    cJSON *bool_replacement = cJSON_CreateBool(second_is_string);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(array, 1, bool_replacement);
    cJSON *extra_num = cJSON_CreateNumber(5.0 + (double)first_is_number);
    cJSON_bool added_extra = cJSON_AddItemToArray(array, extra_num);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    (void)added_num;
    (void)added_str;
    (void)added_array;
    (void)first_is_number;
    (void)second_is_string;
    (void)replaced;
    (void)added_extra;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}