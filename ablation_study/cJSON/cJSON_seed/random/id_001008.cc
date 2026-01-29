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
//<ID> 1008
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
    const char *s0 = "first";
    const char *s1 = "second";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON_bool add_arr_ok = (cJSON_bool)0;
    cJSON_bool add_obj_ok = (cJSON_bool)0;
    char *serialized = (char *)0;
    const char *retrieved0 = (const char *)0;
    cJSON_bool flag_is_true = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, two string items and a true boolean item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString(s0);
    str1 = cJSON_CreateString(s1);
    true_item = cJSON_CreateTrue();

    // step 3: Configure - attach strings to the array and attach array and boolean to the root object
    add_arr_ok = cJSON_AddItemToArray(arr, str0);
    add_arr_ok = (cJSON_bool)(add_arr_ok + (cJSON_bool)cJSON_AddItemToArray(arr, str1));
    add_obj_ok = cJSON_AddItemToObject(root, array_key, arr);
    add_obj_ok = (cJSON_bool)(add_obj_ok + (cJSON_bool)cJSON_AddItemToObject(root, flag_key, true_item));

    // step 4: Operate - serialize and retrieve the first array element
    serialized = cJSON_Print(root);
    fetched0 = cJSON_GetArrayItem(arr, 0);
    retrieved0 = cJSON_GetStringValue(fetched0);
    flag_is_true = cJSON_IsTrue(true_item);

    // step 5: Validate - compute a small validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str0 != (cJSON *)0) + (int)(str1 != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(add_arr_ok) + (int)(add_obj_ok) + (int)(serialized != (char *)0) + (int)(retrieved0 != (const char *)0 && retrieved0[0] == 'f') + (int)(flag_is_true);
    (void)validation_score;

    // step 6: Cleanup - free serialized string and delete root (which frees attached items)
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}