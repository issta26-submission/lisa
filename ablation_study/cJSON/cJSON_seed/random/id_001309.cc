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
//<ID> 1309
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
    cJSON *flag = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array, two string items and a false flag
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("one");
    item2 = cJSON_CreateString("two");
    flag = cJSON_CreateFalse();

    // step 3: Configure - add a greeting string to root, attach the array and the flag to root, populate the array
    greeting = cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 4: Operate - retrieve the array and flag using case-sensitive lookup and inspect them
    retrieved_arr = cJSON_GetObjectItemCaseSensitive(root, "items");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    flag_is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(greeting != (cJSON *)0) + (int)(arr_size == 2) + (int)(flag_is_false != 0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}