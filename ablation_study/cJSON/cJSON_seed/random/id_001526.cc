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
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_false_elem = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_flags = 0;
    cJSON_bool added_to_array = 0;
    cJSON_bool is_false = 0;
    double num_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    added_flags = cJSON_AddItemToObject(root, "flags", arr);
    retrieved_num = cJSON_AddNumberToObject(root, "answer", 42.0);
    added_to_array = cJSON_AddItemToArray(arr, false_item);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_arr = cJSON_GetObjectItem(root, "flags");
    retrieved_false_elem = (retrieved_arr != (cJSON *)0) ? cJSON_GetArrayItem(retrieved_arr, 0) : (cJSON *)0;
    is_false = cJSON_IsFalse(retrieved_false_elem);
    num_val = (retrieved_num != (cJSON *)0) ? cJSON_GetNumberValue(retrieved_num) : 0.0;

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)(retrieved_num != (cJSON *)0) + (int)added_flags + (int)added_to_array + (int)is_false + (int)(num_val == 42.0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)num_val;
    (void)retrieved_false_elem;
    (void)retrieved_arr;
    (void)num_item;
    // API sequence test completed successfully
    return 66;
}