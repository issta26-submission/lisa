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
//<ID> 1382
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
    cJSON *n1 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *bfalse = (cJSON *)0;
    cJSON *found_arr = (cJSON *)0;
    cJSON *found_false = (cJSON *)0;
    int arr_size = 0;
    int is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize - create object, array and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(42.0);
    s1 = cJSON_CreateString("example");
    bfalse = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach to root
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, bfalse);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate - retrieve the array, measure size, inspect the third element for false
    found_arr = cJSON_GetObjectItem(root, "payload");
    arr_size = cJSON_GetArraySize(found_arr);
    found_false = cJSON_GetArrayItem(found_arr, 2);
    is_false_flag = (int)cJSON_IsFalse(found_false);

    // step 5: Validate - produce a simple validation score
    validation_score = (int)(root != (cJSON *)0) + (int)(arr_size == 3) + is_false_flag;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    (void)found_arr;
    (void)found_false;
    return 66;
}