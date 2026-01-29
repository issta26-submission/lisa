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
//<ID> 1304
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
    cJSON *flag = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and a false flag
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    flag = cJSON_CreateFalse();

    // step 3: Configure - attach array and flag to root and add a string property
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddStringToObject(root, "greeting", "hello");

    // step 4: Operate - populate the array and then retrieve items case-sensitively
    cJSON_AddItemToArray(arr, cJSON_CreateString("one"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    retrieved_arr = cJSON_GetObjectItemCaseSensitive(root, "list");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    flag_is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate - derive a simple validation score from the results
    validation_score = (int)(root != (cJSON *)0) + arr_size + (int)flag_is_false + (int)(retrieved_flag != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}