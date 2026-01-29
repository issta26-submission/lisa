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
//<ID> 1383
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *falsy = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_first = (cJSON *)0;
    cJSON *count_item = (cJSON *)0;
    cJSON *has_false_item = (cJSON *)0;
    char *unformatted = (char *)0;
    int arr_size = 0;
    int is_false_flag = 0;

    // step 2: Initialize - build root object and an array with a false and two strings
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    falsy = cJSON_CreateFalse();
    str1 = cJSON_CreateString("one");
    str2 = cJSON_CreateString("two");
    cJSON_AddItemToArray(arr, falsy);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);

    // step 3: Configure - attach the array to the root object and create a child object
    cJSON_AddItemToObject(root, "things", arr);

    // step 4: Operate - retrieve the array, inspect its size and the first element's falseness, and record results into root
    found_arr = cJSON_GetObjectItem(root, "things");
    arr_size = cJSON_GetArraySize(found_arr);
    found_first = cJSON_GetArrayItem(found_arr, 0);
    is_false_flag = (int)cJSON_IsFalse(found_first);
    count_item = cJSON_CreateNumber((double)arr_size);
    cJSON_AddItemToObject(root, "count", count_item);
    has_false_item = cJSON_CreateBool((cJSON_bool)is_false_flag);
    cJSON_AddItemToObject(root, "has_false", has_false_item);
    unformatted = cJSON_PrintUnformatted(root);

    // step 5: Cleanup - free printed buffer and delete the constructed cJSON tree
    if (unformatted) {
        cJSON_free(unformatted);
    }
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)found_arr;
    (void)found_first;
    (void)count_item;
    (void)has_false_item;
    (void)arr_size;
    (void)is_false_flag;
    return 66;
}