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
//<ID> 1173
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
    char *serialized = (char *)0;
    cJSON_bool is_fetched_true = 0;
    cJSON_bool is_bool_item_true = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and attach an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", arr);

    // step 3: Configure - create boolean items and add them to the array
    true_item = cJSON_CreateTrue();
    bool_item = cJSON_CreateBool((cJSON_bool)0);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate - retrieve first array element and evaluate truthiness
    fetched0 = cJSON_GetArrayItem(arr, 0);
    is_fetched_true = cJSON_IsTrue(fetched0);
    is_bool_item_true = cJSON_IsTrue(bool_item);

    // step 5: Validate - compute a simple validation score and serialize for a round-trip
    validation_score = (int)is_fetched_true + (int)is_bool_item_true + cJSON_GetArraySize(arr);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}