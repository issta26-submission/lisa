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
//<ID> 903
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char * arr_key = "my_array";
    cJSON * root = (cJSON *)0;
    cJSON * arr = (cJSON *)0;
    cJSON * false_item = (cJSON *)0;
    cJSON * true_item = (cJSON *)0;
    cJSON * retrieved_item = (cJSON *)0;
    cJSON_bool add_result = (cJSON_bool)0;
    cJSON_bool is_false_result = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize root and array, create boolean items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    true_item = cJSON_CreateTrue();

    // step 3: Populate array with the boolean items
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToArray(arr, true_item);

    // step 4: Attach array to root object
    add_result = cJSON_AddItemToObject(root, arr_key, arr);

    // step 5: Operate - retrieve first element from the array and check if it is false
    retrieved_item = cJSON_GetArrayItem(arr, 0);
    is_false_result = cJSON_IsFalse(retrieved_item);

    // step 6: Validate and Cleanup
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(false_item != (cJSON *)0) + (int)add_result + (int)is_false_result + (int)(retrieved_item != (cJSON *)0);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}