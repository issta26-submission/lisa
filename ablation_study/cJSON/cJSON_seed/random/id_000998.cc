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
//<ID> 998
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *num_key = "pi";
    const char *bool_key = "enabled";
    double pi_value = 3.141592653589793;
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *num_item2 = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *serialized = (char *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    cJSON_bool add_to_array_ok = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - attach array to root, create numbers and add them, add a boolean member
    add_ok1 = cJSON_AddItemToObject(root, arr_key, arr);
    num_item = cJSON_CreateNumber(pi_value);
    add_to_array_ok = cJSON_AddItemToArray(arr, num_item);
    num_item2 = cJSON_CreateNumber(42.0);
    add_ok2 = cJSON_AddItemToObject(root, num_key, num_item2);
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);

    // step 4: Operate - inspect item type and serialize the structure
    is_raw = cJSON_IsRaw(num_item);
    serialized = cJSON_Print(root);
    first_elem = cJSON_GetArrayItem(arr, 0);

    // step 5: Validate - compute a simple validation score using API results
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok1) + (int)(add_to_array_ok) + (int)(add_ok2) + (int)(bool_item != (cJSON *)0) + (int)(serialized != (char *)0) + (int)(first_elem == num_item) + (int)(is_raw == 0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}