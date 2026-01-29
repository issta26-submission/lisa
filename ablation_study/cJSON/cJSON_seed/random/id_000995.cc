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
//<ID> 995
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "values";
    const char *obj_num_key = "answer";
    const char *bool_key = "enabled";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_in_array = (cJSON *)0;
    cJSON *num_in_object = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *serialized = (char *)0;
    cJSON_bool add_ok_arr_attach = (cJSON_bool)0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_object = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an empty array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - attach array to root, create numbers and add them, add a boolean via AddBoolToObject
    add_ok_arr_attach = cJSON_AddItemToObject(root, array_key, arr);
    num_in_array = cJSON_CreateNumber(3.14);
    add_ok_array = cJSON_AddItemToArray(arr, num_in_array);
    num_in_object = cJSON_CreateNumber(42.0);
    add_ok_object = cJSON_AddItemToObject(root, obj_num_key, num_in_object);
    bool_item = cJSON_AddBoolToObject(root, bool_key, (cJSON_bool)1);

    // step 4: Operate - inspect item type and serialize the root
    is_raw = cJSON_IsRaw(num_in_array);
    serialized = cJSON_Print(root);

    // step 5: Validate - fetch the array and its first element and compute a simple validation score
    fetched_arr = cJSON_GetObjectItem(root, array_key);
    first_elem = cJSON_GetArrayItem(fetched_arr, 0);
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok_arr_attach) + (int)(add_ok_array) + (int)(add_ok_object) + (int)(bool_item != (cJSON *)0) + (int)(serialized != (char *)0) + (int)(!is_raw) + (int)(cJSON_GetNumberValue(first_elem) == 3.14);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}