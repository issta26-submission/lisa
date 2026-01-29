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
//<ID> 1000
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
    cJSON *fetched = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_str = (cJSON_bool)0;
    cJSON_bool add_ok_true = (cJSON_bool)0;
    const char *retrieved_str = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a string item and a true boolean
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString(text);
    true_item = cJSON_CreateTrue();

    // step 3: Configure - attach the array to root and populate it, also attach the boolean to root
    add_ok_arr = cJSON_AddItemToObject(root, array_key, arr);
    add_ok_str = cJSON_AddItemToArray(arr, str_item);
    add_ok_true = cJSON_AddItemToObject(root, flag_key, true_item);

    // step 4: Operate - fetch the first array element and read its string value
    fetched = cJSON_GetArrayItem(arr, 0);
    retrieved_str = cJSON_GetStringValue(fetched);

    // step 5: Validate - compute a simple validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_str) + (int)(add_ok_true) + (int)(fetched != (cJSON *)0) + (int)(retrieved_str != (const char *)0 && retrieved_str[0] == 'h');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}