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
//<ID> 1003
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *array_key = "data";
    const char *label_key = "label";
    const char *text = "hello";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *fetched_item0 = (cJSON *)0;
    cJSON *fetched_item1 = (cJSON *)0;
    const char *fetched_str = (const char *)0;
    cJSON_bool added_arr_ok = (cJSON_bool)0;
    cJSON_bool added_label_ok = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, an array, a string item and a true item
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString(text);
    true_item = cJSON_CreateTrue();

    // step 3: Configure - populate array and attach to root; also add a label string to root
    (void)cJSON_AddItemToArray(arr, str_item);
    (void)cJSON_AddItemToArray(arr, true_item);
    added_arr_ok = cJSON_AddItemToObject(root, array_key, arr);
    label_item = cJSON_CreateString("greeting");
    added_label_ok = cJSON_AddItemToObject(root, label_key, label_item);

    // step 4: Operate - retrieve array elements using GetArrayItem and obtain the string value
    fetched_item0 = cJSON_GetArrayItem(arr, 0);
    fetched_item1 = cJSON_GetArrayItem(arr, 1);
    fetched_str = (const char *)cJSON_GetStringValue(fetched_item0);

    // step 5: Validate - compute a small validation score exercising returned values
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(arr != (cJSON *)0)
                     + (int)(str_item != (cJSON *)0)
                     + (int)(true_item != (cJSON *)0)
                     + (int)added_arr_ok
                     + (int)added_label_ok
                     + (int)(fetched_item0 != (cJSON *)0)
                     + (int)(fetched_item1 != (cJSON *)0)
                     + (int)(fetched_str != (const char *)0)
                     + (int)cJSON_IsTrue(fetched_item1);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}