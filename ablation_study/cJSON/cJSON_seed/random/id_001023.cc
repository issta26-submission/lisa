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
//<ID> 1023
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"values\": [1, 2.5]}";
    const char *array_key = "values";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *third = (cJSON *)0;
    cJSON *new_num = (cJSON *)0;
    cJSON_bool is_array = (cJSON_bool)0;
    double val_first = 0.0;
    double val_third = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse JSON into cJSON structure and obtain the array
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, array_key);

    // step 3: Configure - create a new number and append it to the parsed array
    new_num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, new_num);

    // step 4: Operate - check that the item is an array and fetch elements
    is_array = cJSON_IsArray(arr);
    first = cJSON_GetArrayItem(arr, 0);
    third = cJSON_GetArrayItem(arr, 2);
    val_first = cJSON_GetNumberValue(first);
    val_third = cJSON_GetNumberValue(third);

    // step 5: Validate - compute a small validation score exercising the values and type checks
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(new_num != (cJSON *)0) + (int)(is_array) + (int)(val_first == 1.0) + (int)(val_third > 3.0);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}