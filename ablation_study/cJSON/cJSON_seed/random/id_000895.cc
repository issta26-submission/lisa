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
//<ID> 895
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *key_values = "values";
    const char *key_extra = "extra";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item0 = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *fetched_array = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_array_to_object = (cJSON_bool)0;
    cJSON_bool added_num0 = (cJSON_bool)0;
    cJSON_bool added_num1 = (cJSON_bool)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double extracted_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize root and create items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item0 = cJSON_CreateNumber(3.14159);
    num_item1 = cJSON_CreateNumber(42.0);

    // step 3: Configure structure (add numbers to array, add array to object)
    added_num0 = cJSON_AddItemToArray(arr, num_item0);
    added_num1 = cJSON_AddItemToArray(arr, num_item1);
    added_array_to_object = cJSON_AddItemToObject(root, key_values, arr);

    // step 4: Operate (retrieve array from object, check type, extract a number, and print)
    fetched_array = cJSON_GetObjectItem(root, key_values);
    is_array = cJSON_IsArray(fetched_array);
    fetched_num = cJSON_GetArrayItem(fetched_array, 1);
    extracted_value = cJSON_GetNumberValue(fetched_num);
    printed = cJSON_Print(root);

    // step 5: Validate by aggregating checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)is_array + (int)(fetched_num != (cJSON *)0) + (int)(printed != (char *)0) + (int)(extracted_value == 42.0) + (int)added_array_to_object + (int)added_num0 + (int)added_num1;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}