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
//<ID> 1336
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
    cJSON *s0 = (cJSON *)0;
    cJSON *f1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON *elem1 = (cJSON *)0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and two items (string and false)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("first");
    f1 = cJSON_CreateFalse();

    // step 3: Configure - insert items into the array and attach array to root
    cJSON_InsertItemInArray(arr, 0, s0);
    cJSON_InsertItemInArray(arr, 1, f1);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate - retrieve the array from root and inspect the second element for false
    retrieved_arr = cJSON_GetObjectItem(root, "payload");
    elem1 = cJSON_GetArrayItem(retrieved_arr, 1);
    is_false = cJSON_IsFalse(elem1);

    // step 5: Validate - compute a simple validation score from results
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(elem1 != (cJSON *)0) + (int)(is_false != 0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}