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
//<ID> 1004
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *text = "true_string";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok0 = (cJSON_bool)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    const char *retrieved_str = (const char *)0;
    cJSON_bool is_true = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create a string and a true boolean, add the array to the root and populate the array
    str_item = cJSON_CreateString(text);
    true_item = cJSON_CreateTrue();
    add_ok_arr = cJSON_AddItemToObject(root, arr_key, arr);
    add_ok0 = cJSON_AddItemToArray(arr, str_item);
    add_ok1 = cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate - retrieve items from the array
    fetched0 = cJSON_GetArrayItem(arr, 0);
    fetched1 = cJSON_GetArrayItem(arr, 1);
    retrieved_str = cJSON_GetStringValue(fetched0);
    is_true = cJSON_IsTrue(fetched1);

    // step 5: Validate - compute a small validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok0) + (int)(add_ok1) + (int)(fetched0 != (cJSON *)0) + (int)(fetched1 != (cJSON *)0) + (int)(retrieved_str != (const char *)0 && retrieved_str[0] == 't') + (int)(is_true == 1);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}