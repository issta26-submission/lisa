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
//<ID> 997
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "numbers";
    const char *num_key = "constant";
    const char *flag_key = "valid";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *num_obj_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *serialized = (char *)0;
    cJSON_bool add_arr_ok = (cJSON_bool)0;
    cJSON_bool add_item_to_array_ok = (cJSON_bool)0;
    cJSON_bool add_num_to_obj_ok = (cJSON_bool)0;
    cJSON_bool is_raw = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array to hold numbers
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_arr_ok = cJSON_AddItemToObject(root, arr_key, arr);

    // step 3: Configure - create numeric items and add them to array and object, add a boolean via helper
    num_item = cJSON_CreateNumber(3.1415);
    add_item_to_array_ok = cJSON_AddItemToArray(arr, num_item);
    num_obj_item = cJSON_CreateNumber(2.718);
    add_num_to_obj_ok = cJSON_AddItemToObject(root, num_key, num_obj_item);
    bool_item = cJSON_AddBoolToObject(root, flag_key, (cJSON_bool)1);

    // step 4: Operate - serialize, fetch inserted items, inspect first element for RAW status and minify the string
    serialized = cJSON_Print(root);
    fetched_arr = cJSON_GetObjectItem(root, arr_key);
    first_elem = cJSON_GetArrayItem(fetched_arr, 0);
    is_raw = cJSON_IsRaw(first_elem);
    cJSON_Minify(serialized);

    // step 5: Validate - compute a simple validation score exercising returned values and properties
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(add_arr_ok)
                     + (int)(add_item_to_array_ok)
                     + (int)(add_num_to_obj_ok)
                     + (int)(bool_item != (cJSON *)0)
                     + (int)(serialized != (char *)0)
                     + (int)(cJSON_GetNumberValue(first_elem) == 3.1415)
                     + (int)(is_raw == (cJSON_bool)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root to free all attached items
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}