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
//<ID> 1007
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *arr_key = "items";
    const char *flag_key = "enabled";
    const char *s1 = "alpha";
    const char *s2 = "beta";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON_bool add_ok_arr = (cJSON_bool)0;
    cJSON_bool add_ok_arr2 = (cJSON_bool)0;
    cJSON_bool add_ok_flag = (cJSON_bool)0;
    cJSON *fetched = (cJSON *)0;
    const char *fetched_str = (const char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - create string items and attach them to the array, add array and a true flag to root
    item1 = cJSON_CreateString(s1);
    item2 = cJSON_CreateString(s2);
    add_ok_arr = cJSON_AddItemToArray(arr, item1);
    add_ok_arr2 = cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, arr_key, arr);
    true_item = cJSON_CreateTrue();
    add_ok_flag = cJSON_AddItemToObject(root, flag_key, true_item);

    // step 4: Operate - retrieve the second array item and obtain its string value
    fetched = cJSON_GetArrayItem(arr, 1);
    fetched_str = cJSON_GetStringValue(fetched);

    // step 5: Validate - compute a simple validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(item1 != (cJSON *)0) + (int)(item2 != (cJSON *)0) + (int)(add_ok_arr) + (int)(add_ok_arr2) + (int)(add_ok_flag) + (int)(fetched != (cJSON *)0) + (int)(fetched_str != (const char *)0 && fetched_str[0] == 'b');
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}