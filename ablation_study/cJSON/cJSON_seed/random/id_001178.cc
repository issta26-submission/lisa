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
//<ID> 1178
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
    cJSON *b_false = (cJSON *)0;
    cJSON_bool added_t = 0;
    cJSON_bool added_b = 0;
    cJSON_bool t_true_is_true = 0;
    cJSON_bool b_false_is_true = 0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "booleans", arr);

    // step 3: Configure - create a true item and a boolean (false) item and add to array
    t_true = cJSON_CreateTrue();
    b_false = cJSON_CreateBool(0);
    added_t = cJSON_AddItemToArray(arr, t_true);
    added_b = cJSON_AddItemToArray(arr, b_false);

    // step 4: Operate - inspect items using cJSON_IsTrue
    t_true_is_true = cJSON_IsTrue(t_true);
    b_false_is_true = cJSON_IsTrue(b_false);

    // step 5: Validate - compute a simple validation score based on adds and truth checks
    validation_score = (int)t_true_is_true + (int)b_false_is_true + (int)added_t + (int)added_b;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}