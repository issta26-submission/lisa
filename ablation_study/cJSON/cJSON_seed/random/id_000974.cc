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
//<ID> 974
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *single_key = "single";
    const char *flt_key = "float_array";
    float float_vals[3];
    memset(float_vals, 0, sizeof(float_vals));
    float_vals[0] = 1.5f;
    float_vals[1] = 2.5f;
    float_vals[2] = 3.5f;
    int float_count = 3;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    cJSON *obj_num = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    char *json_str = (char *)0;
    cJSON *first_elem = (cJSON *)0;
    double first_val = 0.0;
    int validation_score = 0;

    // step 2: Setup - create container objects and a float array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    float_arr = cJSON_CreateFloatArray(float_vals, float_count);

    // step 3: Configure - create a number, add it to the array, attach array and float array to root, add a number directly to root
    num_item = cJSON_CreateNumber(42.42);
    add_ok = cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToObject(root, arr_key, arr);
    obj_num = cJSON_AddNumberToObject(root, single_key, 1000.001);
    cJSON_AddItemToObject(root, flt_key, float_arr);

    // step 4: Operate - serialize and minify the JSON, then inspect the first array element
    json_str = cJSON_Print(root);
    cJSON_Minify(json_str);
    first_elem = cJSON_GetArrayItem(arr, 0);
    first_val = cJSON_GetNumberValue(first_elem);

    // step 5: Validate - compute a simple validation score that exercises results
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok) + (int)(obj_num != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(first_val == 42.42);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which owns attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}