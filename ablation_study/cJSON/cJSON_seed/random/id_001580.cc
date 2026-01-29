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
//<ID> 1580
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
    cJSON *dup_str = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *first_value = (char *)0;
    cJSON_bool added_first = 0;
    cJSON_bool added_dup = 0;
    cJSON_bool added_arr_to_root = 0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("alpha");

    // step 3: Configure
    added_first = cJSON_AddItemToArray(arr, str_item);
    dup_str = cJSON_Duplicate((const cJSON *)str_item, 1);
    added_dup = cJSON_AddItemToArray(arr, dup_str);
    added_arr_to_root = cJSON_AddItemToObjectCS(root, "letters", arr);

    // step 4: Operate
    retrieved_arr = cJSON_GetObjectItem(root, "letters");
    array_size = cJSON_GetArraySize(retrieved_arr);
    first_elem = cJSON_GetArrayItem(retrieved_arr, 0);
    first_value = cJSON_GetStringValue(first_elem);

    // step 5: Validate
    validation_score = (int)added_first
        + (int)added_dup
        + (int)added_arr_to_root
        + (int)(retrieved_arr != (cJSON *)0)
        + (int)(array_size == 2)
        + (int)(first_elem != (cJSON *)0)
        + (int)(first_value != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)first_value;
    (void)first_elem;
    (void)retrieved_arr;
    (void)dup_str;
    (void)str_item;
    (void)arr;
    (void)root;
    // API sequence test completed successfully
    return 66;
}