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
//<ID> 1177
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
    cJSON *item_true = (cJSON *)0;
    cJSON *item_bool = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    char *serialized = (char *)0;
    int validation_score = 0;
    cJSON_bool is_first_true = 0;

    // step 2: Setup - create root object and an array, attach the array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", arr);

    // step 3: Configure - create boolean items and add them to the array
    item_true = cJSON_CreateTrue();
    item_bool = cJSON_CreateBool(0);
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_bool);

    // step 4: Operate - retrieve the first array element and test if it is true
    fetched0 = cJSON_GetArrayItem(arr, 0);
    is_first_true = cJSON_IsTrue(fetched0);

    // step 5: Validate - compute a simple validation score and serialize the structure
    validation_score = (int)is_first_true + cJSON_GetArraySize(arr);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}