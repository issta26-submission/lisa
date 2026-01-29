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
//<ID> 972
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *container_key = "values";
    const char *direct_num_key = "direct_num";
    float float_values[3] = { 1.25f, 2.5f, 3.75f };
    char json_with_ws[] = " { \"note\" : \" sample \" , \"flag\" : true } ";
    cJSON *root = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *float_arr = (cJSON *)0;
    cJSON *created_number = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_ok3 = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, a container array, a float array and an explicit number item
    root = cJSON_CreateObject();
    container = cJSON_CreateArray();
    float_arr = cJSON_CreateFloatArray(float_values, 3);
    created_number = cJSON_CreateNumber(7.0);

    // step 3: Configure - add the float array and the explicit number into the container array
    add_ok1 = cJSON_AddItemToArray(container, float_arr);
    add_ok2 = cJSON_AddItemToArray(container, created_number);

    // step 4: Operate - attach the container to the root and also add a number directly via helper
    add_ok3 = cJSON_AddItemToObject(root, container_key, container);
    cJSON *direct_num_item = cJSON_AddNumberToObject(root, direct_num_key, 42.0);

    // step 5: Validate - minify a sample JSON string and compute a simple validation score
    cJSON_Minify(json_with_ws);
    validation_score = (int)(root != (cJSON *)0) + (int)(container != (cJSON *)0) + (int)(float_arr != (cJSON *)0) + (int)add_ok1 + (int)add_ok2 + (int)add_ok3 + (int)(direct_num_item != (cJSON *)0) + (int)(json_with_ws[0] == '{');
    (void)validation_score;

    // step 6: Cleanup - delete the root (which frees all attached items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}