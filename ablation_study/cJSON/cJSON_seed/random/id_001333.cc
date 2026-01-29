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
//<ID> 1333
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON_bool is_false0 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateFalse();
    item1 = cJSON_CreateTrue();

    // step 3: Configure - insert items into array
    cJSON_InsertItemInArray(arr, 0, item0);
    cJSON_InsertItemInArray(arr, 1, item1);

    // step 4: Operate - attach array to root using AddItemToObject and add another false item
    cJSON_AddItemToObject(root, "flags", arr);
    cJSON_AddItemToObject(root, "extra_false", cJSON_CreateFalse());

    // step 5: Validate - retrieve array and check first element is false
    retrieved_arr = cJSON_GetObjectItem(root, "flags");
    item0 = cJSON_GetArrayItem(retrieved_arr, 0);
    is_false0 = cJSON_IsFalse(item0);
    validation_score = (int)(root != (cJSON *)0) + (int)(retrieved_arr != (cJSON *)0) + (int)(is_false0 != 0);

    // step 6: Cleanup - delete root to free everything
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}