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
//<ID> 1176
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
    cJSON *t_true = (cJSON *)0;
    cJSON *b_bool = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON_bool is_first_true = 0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure - create a true item and a boolean item and add them to the array
    t_true = cJSON_CreateTrue();
    b_bool = cJSON_CreateBool(0);
    cJSON_AddItemToArray(arr, t_true);
    cJSON_AddItemToArray(arr, b_bool);

    // step 4: Operate / Validate - fetch first array element, test if it's true, serialize and compute score
    fetched0 = cJSON_GetArrayItem(arr, 0);
    is_first_true = cJSON_IsTrue(fetched0);
    validation_score = (int)is_first_true + cJSON_GetArraySize(arr);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}