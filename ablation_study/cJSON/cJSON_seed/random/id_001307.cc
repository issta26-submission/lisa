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
//<ID> 1307
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *status_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *greeting_str = (char *)0;
    cJSON_bool greeting_is_false = (cJSON_bool)0;
    int arr_size = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, a string child and an array child
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    arr_item1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(arr, arr_item1);
    status_item = cJSON_AddStringToObject(root, "status", "ready");

    // step 3: Operate - retrieve the greeting, inspect it and read array size
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    greeting_str = cJSON_GetStringValue(retrieved);
    greeting_is_false = cJSON_IsFalse(retrieved);
    arr_size = cJSON_GetArraySize(arr);

    // step 4: Validate - compute a simple validation score using results
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved != (cJSON *)0) + (int)(status_item != (cJSON *)0) + (int)(greeting_is_false == 0) + arr_size + (int)(greeting_str[0] == 'h');
    (void)validation_score;

    // step 5: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}