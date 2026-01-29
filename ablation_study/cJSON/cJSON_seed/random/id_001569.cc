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
//<ID> 1569
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
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    char *first_str = (char *)0;
    cJSON_bool added_to_array = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("sample_string");

    // step 3: Configure
    added_to_array = cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    flag_item = cJSON_AddBoolToObject(root, "enabled", 1);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "items");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_str = cJSON_GetStringValue(first_elem);

    // step 5: Validate
    validation_score = (int)added_to_array
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 1)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_str != (char *)0)
        + (int)(flag_item != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_str;
    (void)first_elem;
    (void)retrieved_arr;
    (void)arr;
    (void)str_item;
    (void)flag_item;
    // API sequence test completed successfully
    return 66;
}