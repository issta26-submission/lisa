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
//<ID> 1551
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
    cJSON *str_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *inserted_ptr = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_str_item = (cJSON *)0;
    cJSON_bool added_str_to_arr = 0;
    cJSON_bool inserted_num = 0;
    cJSON_bool added_arr_to_obj = 0;
    char *printed = (char *)0;
    const char *extracted_str = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("first_element");
    num_item = cJSON_CreateNumber(3.14159);

    // step 3: Configure
    added_str_to_arr = cJSON_AddItemToArray(arr, str_item);
    inserted_num = cJSON_InsertItemInArray(arr, 1, num_item);
    added_arr_to_obj = cJSON_AddItemToObject(root, "data", arr);
    inserted_ptr = cJSON_GetArrayItem(arr, 1);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "data");
    retrieved_str_item = cJSON_GetArrayItem(retrieved_arr, 0);
    extracted_str = cJSON_GetStringValue(retrieved_str_item);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_str_to_arr
        + (int)inserted_num
        + (int)added_arr_to_obj
        + (int)(inserted_ptr == num_item)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(extracted_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)extracted_str;
    (void)retrieved_arr;
    (void)retrieved_str_item;
    (void)inserted_ptr;
    (void)num_item;
    (void)str_item;
    // API sequence test completed successfully
    return 66;
}