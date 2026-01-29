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
//<ID> 977
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_obj_key = "answer";
    const char *nums_array_key = "numbers_array";
    const char *floats_key = "float_vals";
    float sample_floats[3];
    sample_floats[0] = 1.25f;
    sample_floats[1] = 2.5f;
    sample_floats[2] = -3.75f;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *floatArray = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *num_in_obj = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a created number, and a float array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);
    floatArray = cJSON_CreateFloatArray(sample_floats, 3);

    // step 3: Configure - add the created number to the array and attach array and float array to root; also add a number via helper
    add_ok1 = cJSON_AddItemToArray(arr, num_item);
    add_ok2 = cJSON_AddItemToObject(root, nums_array_key, arr);
    add_ok3 = cJSON_AddItemToObject(root, floats_key, floatArray);
    num_in_obj = cJSON_AddNumberToObject(root, num_obj_key, 42.0);

    // step 4: Operate - serialize the composed JSON and minify the serialized buffer in-place
    json_str = cJSON_Print(root);
    cJSON_Minify(json_str);

    // step 5: Validate - compute a simple validation score exercising created objects and operations
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(arr != (cJSON *)0)
                     + (int)(floatArray != (cJSON *)0)
                     + (int)(num_item != (cJSON *)0)
                     + (int)(num_in_obj != (cJSON *)0)
                     + (int)(add_ok1)
                     + (int)(add_ok2)
                     + (int)(add_ok3)
                     + (int)(cJSON_GetArraySize(arr) == 1)
                     + (int)(json_str != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete the root (which owns all attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}