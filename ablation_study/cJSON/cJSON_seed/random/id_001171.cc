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
//<ID> 1171
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *f0 = (cJSON *)0;
    cJSON *f2 = (cJSON *)0;
    cJSON_bool is_f0_true = 0;
    cJSON_bool is_f2_true = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and attach an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "booleans", arr);

    // step 3: Configure - create boolean items and add them to the array
    item0 = cJSON_CreateTrue();
    item1 = cJSON_CreateBool(0);
    item2 = cJSON_CreateBool(1);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);

    // step 4: Operate - fetch specific items and inspect truthiness
    f0 = cJSON_GetArrayItem(arr, 0);
    f2 = cJSON_GetArrayItem(arr, 2);
    is_f0_true = cJSON_IsTrue(f0);
    is_f2_true = cJSON_IsTrue(f2);
    validation_score = (int)is_f0_true + (int)is_f2_true + cJSON_GetArraySize(arr);

    // step 5: Cleanup - delete root which frees contained items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}