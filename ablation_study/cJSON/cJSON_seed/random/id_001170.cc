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
//<ID> 1170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = (cJSON *)0;
    cJSON *flags = (cJSON *)0;
    cJSON *t_true = (cJSON *)0;
    cJSON *b_false = (cJSON *)0;
    cJSON *b_true = (cJSON *)0;
    cJSON_bool is_t_true = 0;
    cJSON_bool is_b_false_true = 0;
    cJSON_bool is_b_true_true = 0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Setup - create root object and an array for boolean flags
    root = cJSON_CreateObject();
    flags = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", flags);

    // step 3: Configure - create boolean items and add them to the array
    t_true = cJSON_CreateTrue();
    b_false = cJSON_CreateBool(0);
    b_true = cJSON_CreateBool(1);
    cJSON_AddItemToArray(flags, t_true);
    cJSON_AddItemToArray(flags, b_false);
    cJSON_AddItemToArray(flags, b_true);

    // step 4: Operate - inspect items using cJSON_IsTrue
    is_t_true = cJSON_IsTrue(t_true);
    is_b_false_true = cJSON_IsTrue(b_false);
    is_b_true_true = cJSON_IsTrue(b_true);

    // step 5: Validate - compute a simple validation score and serialize the structure
    validation_score = (int)is_t_true + (int)is_b_false_true + (int)is_b_true_true + cJSON_GetArraySize(flags);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}