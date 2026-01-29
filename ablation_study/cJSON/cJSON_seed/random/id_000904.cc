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
//<ID> 904
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "arr";
    const char *flag_key = "flag";
    double num0 = 10.0;
    double num1 = 20.0;
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *num_item0 = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    cJSON *retrieved_array = (cJSON *)0;
    cJSON *retrieved_elem = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool was_false = (cJSON_bool)0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    num_item0 = cJSON_CreateNumber(num0);
    num_item1 = cJSON_CreateNumber(num1);
    flag_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(array, num_item0);
    cJSON_AddItemToArray(array, num_item1);
    cJSON_AddItemToObject(root, arr_key, array);
    cJSON_AddItemToObject(root, flag_key, flag_item);

    // step 4: Operate
    retrieved_array = cJSON_GetObjectItem(root, arr_key);
    retrieved_elem = cJSON_GetArrayItem(retrieved_array, 1);
    retrieved_flag = cJSON_GetObjectItem(root, flag_key);
    was_false = cJSON_IsFalse(retrieved_flag);
    extracted_value = cJSON_GetNumberValue(retrieved_elem);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)(array != (cJSON *)0) + (int)(retrieved_array != (cJSON *)0) + (int)(retrieved_elem != (cJSON *)0) + (int)was_false + (int)(extracted_value == num1);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}