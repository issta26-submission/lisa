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
//<ID> 1301
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
    cJSON *note = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    flag = cJSON_CreateFalse();
    note = cJSON_CreateString("example-note");

    // step 3: Configure - attach array and items to the root and add a labeled string
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddItemToObject(root, "note", note);
    cJSON_AddStringToObject(root, "label", "test-label");

    // step 4: Operate - populate the array and retrieve object members case-sensitively
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem"));
    retrieved_arr = cJSON_GetObjectItemCaseSensitive(root, "items");
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    arr_size = cJSON_GetArraySize(retrieved_arr);
    flag_is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate - derive a simple validation score from results
    validation_score = arr_size + (int)flag_is_false + (int)(retrieved_arr != (cJSON *)0) + (int)(retrieved_flag != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}