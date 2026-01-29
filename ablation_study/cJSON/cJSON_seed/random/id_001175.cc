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
//<ID> 1175
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    int validation_score = 0;
    cJSON_bool check0 = 0;
    cJSON_bool check1 = 0;

    // step 2: Setup - create root object and attach an array member
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure - create true values (one via CreateTrue, one via CreateBool) and add to array
    true_item = cJSON_CreateTrue();
    bool_item = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate / Validate - fetch first element and verify both are true
    fetched0 = cJSON_GetArrayItem(arr, 0);
    check0 = cJSON_IsTrue(fetched0);
    check1 = cJSON_IsTrue(bool_item);
    validation_score = (int)check0 + (int)check1 + cJSON_GetArraySize(arr);

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}