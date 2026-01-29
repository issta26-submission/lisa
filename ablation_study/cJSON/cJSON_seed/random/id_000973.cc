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
//<ID> 973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "my_collection";
    const char *num_key = "root_number";
    const float sample_floats[3] = { 1.5f, -2.25f, 3.75f };
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *created_num = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a float array and an explicit number
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    float_arr = cJSON_CreateFloatArray(sample_floats, 3);
    created_num = cJSON_CreateNumber(42.42);

    // step 3: Configure - add the created number and the float array into the array, then attach array to root
    add_ok1 = cJSON_AddItemToArray(arr, created_num);
    add_ok2 = cJSON_AddItemToArray(arr, float_arr);
    cJSON_AddItemToObject(root, arr_key, arr);

    // step 4: Operate - add a number directly into the root using AddNumberToObject, serialize and minify
    added_num = cJSON_AddNumberToObject(root, num_key, -7.125);
    json_str = cJSON_PrintUnformatted(root);
    cJSON_Minify(json_str);

    // step 5: Validate - inspect array size and produce a simple validation score
    arr_size = cJSON_GetArraySize(arr);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)(created_num != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)(added_num != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(arr_size == 2);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which frees all attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}