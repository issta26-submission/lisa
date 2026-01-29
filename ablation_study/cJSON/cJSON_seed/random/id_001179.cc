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
//<ID> 1179
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
    cJSON *fetched1 = (cJSON *)0;
    char *serialized = (char *)0;
    cJSON_bool is_first_true = 0;
    cJSON_bool is_second_true = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", arr);

    // step 3: Configure - create boolean items (one via CreateTrue and one via CreateBool) and add them to the array
    true_item = cJSON_CreateTrue();
    bool_item = cJSON_CreateBool(0);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate - retrieve array items and inspect them with cJSON_IsTrue, serialize for validation
    fetched0 = cJSON_GetArrayItem(arr, 0);
    fetched1 = cJSON_GetArrayItem(arr, 1);
    is_first_true = cJSON_IsTrue(fetched0);
    is_second_true = cJSON_IsTrue(fetched1);
    validation_score = (int)is_first_true + (int)(!is_second_true) + cJSON_GetArraySize(arr);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}