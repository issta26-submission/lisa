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
//<ID> 192
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
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *null_item = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *flag_first_is_number = (cJSON *)0;
    cJSON *flag_second_is_string = (cJSON *)0;
    char *json_text = (char *)0;
    int arr_size_after_delete = 0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    cJSON_bool second_is_string = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    num_item = cJSON_CreateNumber(42.0);
    str_item = cJSON_CreateString("hello");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, null_item);
    cJSON_AddItemToArray(arr, str_item);

    // step 4: Operate
    cJSON_DeleteItemFromArray(arr, 1);

    // step 5: Validate
    arr_size_after_delete = cJSON_GetArraySize(arr);
    count_item = cJSON_CreateNumber((double)arr_size_after_delete);
    cJSON_AddItemToObject(root, "count", count_item);
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    first_is_number = cJSON_IsNumber(first);
    second_is_string = cJSON_IsString(second);
    flag_first_is_number = cJSON_CreateBool(first_is_number);
    flag_second_is_string = cJSON_CreateBool(second_is_string);
    cJSON_AddItemToObject(root, "first_is_number", flag_first_is_number);
    cJSON_AddItemToObject(root, "second_is_string", flag_second_is_string);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}