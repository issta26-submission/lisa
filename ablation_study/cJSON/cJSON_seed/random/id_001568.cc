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
//<ID> 1568
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
    cJSON *str_a = (cJSON *)0;
    cJSON *str_b = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_first = (cJSON *)0;
    const char *first_str = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool added_arr_to_obj = 0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_a = cJSON_CreateString("hello");
    str_b = cJSON_CreateString("world");

    // step 3: Configure
    added_a = cJSON_AddItemToArray(arr, str_a);
    added_b = cJSON_AddItemToArray(arr, str_b);
    added_arr_to_obj = cJSON_AddItemToObject(root, "list", arr);
    bool_item = cJSON_AddBoolToObject(root, "valid", (cJSON_bool)1);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_first = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(retrieved_first);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_a
        + (int)added_b
        + (int)added_arr_to_obj
        + (int)(bool_item != (cJSON *)0)
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(arr_size == 2)
        + (int)(retrieved_first != (cJSON *)0)
        + (int)(first_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_str;
    (void)retrieved_first;
    (void)retrieved_arr;
    (void)str_a;
    (void)str_b;
    (void)bool_item;
    return 66;
    // API sequence test completed successfully
}