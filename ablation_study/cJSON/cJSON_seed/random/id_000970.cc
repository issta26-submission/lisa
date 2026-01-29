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
//<ID> 970
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *num_key = "pi";
    const char *float_array_key = "floats";
    const char *set_key = "set";
    float init_floats[3];
    init_floats[0] = 1.0f;
    init_floats[1] = 2.5f;
    init_floats[2] = -3.75f;
    char json_buf[] = "  { \"a\" : 123 , \"b\" : [ 1, 2 , 3 ] }  ";
    cJSON *root = (cJSON *)0;
    cJSON *float_array = (cJSON *)0;
    cJSON *parent_array = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *num_in_obj = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    int array_size = 0;
    int validation_score = 0;

    // step 2: Setup - create root object, a float array from C floats, and a standalone number
    root = cJSON_CreateObject();
    float_array = cJSON_CreateFloatArray(init_floats, 3);
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure - append an extra number to the float array and add the standalone number into a parent array
    add_ok1 = cJSON_AddItemToArray(float_array, cJSON_CreateNumber(-1.5));
    parent_array = cJSON_CreateArray();
    add_ok2 = cJSON_AddItemToArray(parent_array, num_item);

    // step 4: Operate - add a number to the root using AddNumberToObject and attach arrays to the root
    num_in_obj = cJSON_AddNumberToObject(root, num_key, 3.14159);
    add_ok3 = cJSON_AddItemToObject(root, float_array_key, float_array);
    (void)cJSON_AddItemToObject(root, set_key, parent_array);

    // step 5: Validate - minify an external JSON buffer and compute a simple validation score that exercises results
    cJSON_Minify(json_buf);
    array_size = cJSON_GetArraySize(float_array);
    validation_score = (int)(root != (cJSON *)0) + (int)(num_in_obj != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)add_ok3 + (int)(array_size == 4) + (int)(json_buf[0] == '{');
    (void)validation_score;

    // step 6: Cleanup - delete the root (which frees all attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}