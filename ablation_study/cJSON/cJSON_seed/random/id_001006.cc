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
//<ID> 1006
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "items";
    const char *flag_key = "enabled";
    const char *text = "hello";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched_item = (cJSON *)0;
    const char *retrieved_str = (const char *)0;
    cJSON_bool add_ok_array = (cJSON_bool)0;
    cJSON_bool add_ok_arr_to_obj = (cJSON_bool)0;
    cJSON_bool add_ok_true_to_obj = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a string item and a true boolean item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString(text);
    true_item = cJSON_CreateTrue();

    // step 3: Configure - attach the string to the array and attach array and boolean to the root object
    add_ok_array = cJSON_AddItemToArray(arr, str_item);
    add_ok_arr_to_obj = cJSON_AddItemToObject(root, array_key, arr);
    add_ok_true_to_obj = cJSON_AddItemToObject(root, flag_key, true_item);

    // step 4: Operate - retrieve the first element of the array and get its string value
    fetched_item = cJSON_GetArrayItem(arr, 0);
    retrieved_str = cJSON_GetStringValue(fetched_item);

    // step 5: Validate - compute a simple validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(add_ok_array) + (int)(add_ok_arr_to_obj) + (int)(add_ok_true_to_obj) + (int)(retrieved_str != (const char *)0 && retrieved_str[0] == 'h');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached array and items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}