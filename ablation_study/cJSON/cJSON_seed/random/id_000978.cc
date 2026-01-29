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
//<ID> 978
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *float_key = "floats";
    const char *count_key = "count";
    float float_values[3] = {1.5f, 2.5f, 3.5f};
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and arrays
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    float_arr = cJSON_CreateFloatArray(float_values, 3);

    // step 3: Configure - create a number and add it to the array, then attach the array and float array to the root; also add a number via helper
    num_item = cJSON_CreateNumber(42.0);
    add_ok1 = cJSON_AddItemToArray(arr, num_item);
    add_ok2 = cJSON_AddItemToObject(root, arr_key, arr);
    added_num = cJSON_AddNumberToObject(root, count_key, 3.0);
    cJSON_AddItemToObject(root, float_key, float_arr);

    // step 4: Operate - serialize the constructed JSON and minify it in place
    json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);

    // step 5: Validate - compute a simple validation score exercising results
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(added_num != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which owns attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}