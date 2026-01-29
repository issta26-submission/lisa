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
//<ID> 1380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_element = (cJSON *)0;
    cJSON *found_flag = (cJSON *)0;
    const char *elem_str = (const char *)0;
    int arr_size = 0;
    int is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array with two strings, and a false flag
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("one");
    item2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "numbers", arr);
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "active", item_false);

    // step 3: Configure - add metadata object and a duplicated element as a named child
    meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "note", "test payload");
    cJSON_AddItemToObject(root, "meta", meta);
    dup = cJSON_Duplicate(item2, 1);
    cJSON_AddItemToObject(root, "copied_second", dup);

    // step 4: Operate - retrieve the array and flag, get array size and an element string, check false status
    found_arr = cJSON_GetObjectItem(root, "numbers");
    arr_size = cJSON_GetArraySize(found_arr);
    found_element = cJSON_GetArrayItem(found_arr, 1);
    elem_str = cJSON_GetStringValue(found_element);
    found_flag = cJSON_GetObjectItem(root, "active");
    is_false_flag = (int)cJSON_IsFalse(found_flag);

    // step 5: Validate - compute a simple validation score from observations
    validation_score = (int)(root != (cJSON *)0) + (arr_size == 2) + (elem_str != (const char *)0) + is_false_flag + (int)(found_flag != (cJSON *)0);

    // step 6: Cleanup - delete the entire cJSON tree
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)elem_str;
    return 66;
}