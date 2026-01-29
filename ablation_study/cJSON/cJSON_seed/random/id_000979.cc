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
//<ID> 979
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "floats";
    const char *num_key = "answer";
    float numbers[] = { 1.5f, 2.5f, 3.5f };
    cJSON *root = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *added_num_obj = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *first_element = (cJSON *)0;
    cJSON_bool add_ok = (cJSON_bool)0;
    char *json_str = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and a float array from C floats
    root = cJSON_CreateObject();
    float_arr = cJSON_CreateFloatArray(numbers, 3);

    // step 3: Configure - attach the float array to the root, create a number and add it to the array, add a number to the object
    cJSON_AddItemToObject(root, array_key, float_arr);
    num_item = cJSON_CreateNumber(42.0);
    add_ok = cJSON_AddItemToArray(float_arr, num_item);
    added_num_obj = cJSON_AddNumberToObject(root, num_key, 42.0);

    // step 4: Operate - serialize the root and then minify the serialized JSON in place
    json_str = cJSON_Print(root);
    cJSON_Minify(json_str);

    // step 5: Validate - retrieve the array and its first element and compute a simple validation score
    fetched_arr = cJSON_GetObjectItem(root, array_key);
    first_element = cJSON_GetArrayItem(fetched_arr, 0);
    validation_score = (int)(root != (cJSON *)0) + (int)(float_arr == fetched_arr) + (int)add_ok + (int)(added_num_obj != (cJSON *)0) + (int)(json_str != (char *)0) + (int)(cJSON_GetNumberValue(first_element) == (double)numbers[0]);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which cleans up attached items)
    cJSON_free(json_str);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}