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
//<ID> 971
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "my_array";
    const char *num_key = "pi_value";
    float nums[3] = {1.5f, 2.5f, 3.5f};
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON *number_item = (cJSON *)0;
    cJSON *added_number = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create a float array and add it and a number item into the array; also add a number to the root using helper
    float_array = cJSON_CreateFloatArray(nums, 3);
    add_ok1 = cJSON_AddItemToArray(arr, float_array);
    number_item = cJSON_CreateNumber(7.25);
    add_ok2 = cJSON_AddItemToArray(arr, number_item);
    added_number = cJSON_AddNumberToObject(root, num_key, 3.14159);

    // step 4: Operate - attach the array to the root and serialize then minify the resulting JSON
    cJSON_AddItemToObject(root, arr_key, arr);
    json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);

    // step 5: Validate - compute a simple validation score that exercises results
    validation_score = (int)(root != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(float_array == cJSON_GetArrayItem(arr, 0)) + (int)(added_number != (cJSON *)0) + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which frees attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}