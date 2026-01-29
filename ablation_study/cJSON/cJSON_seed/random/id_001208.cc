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
//<ID> 1208
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
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_num = (cJSON *)0;
    int arr_size = 0;
    double num_value = 0.0;
    int validation_score = 0;

    // step 2: Setup - create container and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(123.0);
    str_item = cJSON_CreateString("example_string");
    true_item = cJSON_CreateTrue();

    // step 3: Configure - assemble the JSON structure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 4: Operate - query array size and extract numeric value
    arr_size = cJSON_GetArraySize(arr);
    retrieved_num = cJSON_GetArrayItem(arr, 0);
    num_value = cJSON_GetNumberValue(retrieved_num);

    // step 5: Validate - compute a simple validation score from retrieved data
    validation_score = arr_size + (int)num_value;

    // step 6: Cleanup - free all allocated JSON structures
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}