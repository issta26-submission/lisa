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
//<ID> 1387
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *f = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *third_item = (cJSON *)0;
    char *unformatted = (char *)0;
    int arr_size = 0;
    int is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and three elements (two strings and a false boolean)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    f = cJSON_CreateFalse();

    // step 3: Configure - add the elements to the array and attach the array to the root object
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToArray(arr, f);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - retrieve the array from the object, measure its size and verify the third element is false
    found_arr = cJSON_GetObjectItem(root, "items");
    arr_size = cJSON_GetArraySize(found_arr);
    third_item = cJSON_GetArrayItem(found_arr, 2);
    is_false_flag = (int)cJSON_IsFalse(third_item);
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate & Cleanup - compute a simple validation score, free printed buffer and delete the tree
    validation_score = (int)(root != (cJSON *)0) + (arr_size == 3) + is_false_flag + (unformatted != (char *)0);
    cJSON_free(unformatted);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_arr;
    (void)third_item;
    return 66;
}