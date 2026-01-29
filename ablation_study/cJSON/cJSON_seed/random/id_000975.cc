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
//<ID> 975
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_key = "pi";
    const char *arr_key = "values";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *created_number = (cJSON *)0;
    cJSON *direct_number = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON_bool add_ok_num_to_array = (cJSON_bool)0;
    cJSON_bool add_ok_float_to_array = (cJSON_bool)0;
    cJSON_bool attach_ok = (cJSON_bool)0;
    char json_buf[] = "  { \"sample\" : [  1.0 ,  2.0 , 3.0 ] , \"note\" : \" spaced \" }  ";
    int validation_score = 0;

    // step 2: Setup - create root object and an array container
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create a number (explicit) and a float array, add both to the array
    direct_number = cJSON_CreateNumber(7.0);
    add_ok_num_to_array = cJSON_AddItemToArray(arr, direct_number);
    {
        float nums[3];
        nums[0] = 1.5f;
        nums[1] = 2.5f;
        nums[2] = 3.5f;
        float_array = cJSON_CreateFloatArray(nums, 3);
    }
    add_ok_float_to_array = cJSON_AddItemToArray(arr, float_array);

    // step 4: Operate - add a named number directly into the root object and attach the array to the root
    created_number = cJSON_AddNumberToObject(root, num_key, 3.14159);
    attach_ok = cJSON_AddItemToObject(root, arr_key, arr);

    // step 5: Validate - minify a sample JSON buffer and compute a simple validation score
    cJSON_Minify(json_buf);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(direct_number != (cJSON *)0) + (int)(float_array != (cJSON *)0) + (int)add_ok_num_to_array + (int)add_ok_float_to_array + (int)attach_ok + (int)(created_number != (cJSON *)0) + (int)(json_buf[0] != '\0');
    (void)validation_score;

    // step 6: Cleanup - delete the root (which owns the attached array and created items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}