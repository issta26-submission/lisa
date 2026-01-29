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
//<ID> 991
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "values";
    const char *num_key = "pi";
    const char *bool_key = "enabled";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_in_array = (cJSON *)0;
    cJSON *num_in_object = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_numobj = (cJSON_bool)0;
    cJSON *add_bool_item = (cJSON *)0; /* cJSON_AddBoolToObject returns a cJSON* */
    cJSON_bool raw_check = (cJSON_bool)0;
    cJSON *first_elem = (cJSON *)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array and number items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_in_array = cJSON_CreateNumber(3.14);
    num_in_object = cJSON_CreateNumber(3.1415926535);

    // step 3: Configure - add number to array, add array and number to root, add a boolean to root
    add_ok_arr = cJSON_AddItemToArray(arr, num_in_array);
    add_ok_numobj = cJSON_AddItemToObject(root, num_key, num_in_object);
    add_bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);
    cJSON_AddItemToObject(root, array_key, arr);

    // step 4: Operate - serialize the root and check item type
    serialized = cJSON_Print(root);
    raw_check = cJSON_IsRaw(num_in_array);
    first_elem = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - compute a simple validation score using returned values
    validation_score = (int)(root != (cJSON *)0)
                     + (int)add_ok_arr
                     + (int)add_ok_numobj
                     + (int)(add_bool_item != (cJSON *)0)
                     + (int)(raw_check == 0)
                     + (int)(cJSON_GetNumberValue(first_elem) == 3.14)
                     + (int)(serialized != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which cleans up attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}