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
//<ID> 915
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *num_key = "primary";
    const char *str_key = "label";
    const char *label_value = "example";
    double primary_value = 42.0;
    double extracted_number = 0.0;
    int validation_score = 0;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *arr_elem = (cJSON *)0;
    cJSON_bool is_invalid_flag = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - populate array and attach items to root object
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(32.0));
    cJSON_AddItemToObject(root, arr_key, arr);
    num_item = cJSON_AddNumberToObject(root, num_key, primary_value);
    str_item = cJSON_AddStringToObject(root, str_key, label_value);

    // step 4: Operate - retrieve an array element and extract its numeric value
    arr_elem = cJSON_GetArrayItem(arr, 1);
    extracted_number = cJSON_GetNumberValue(arr_elem);
    is_invalid_flag = cJSON_IsInvalid(str_item);

    // step 5: Validate - simple aggregation of checks into a score
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(extracted_number == 32.0) + (int)(is_invalid_flag == 0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}