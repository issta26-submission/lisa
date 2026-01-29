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
//<ID> 1172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *arr = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *bool_item = (cJSON *)0;
    cJSON_bool is_true_item = 0;
    cJSON_bool is_bool_item = 0;
    int validation_score = 0;

    // step 2: Setup - create an array and boolean items
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    bool_item = cJSON_CreateBool(0);

    // step 3: Configure - add boolean items to the array
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, bool_item);

    // step 4: Operate - inspect boolean values and compute a small validation score
    is_true_item = cJSON_IsTrue(true_item);
    is_bool_item = cJSON_IsTrue(bool_item);
    validation_score = (int)is_true_item + (int)is_bool_item + (int)(arr != (cJSON *)0);

    // step 5: Cleanup - delete the array which frees attached items
    cJSON_Delete(arr);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}