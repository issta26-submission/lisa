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
//<ID> 773
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added = 0;
    cJSON_bool retrieved_is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added = cJSON_AddItemToArray(root, false_item);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(root, 0);
    retrieved_is_false = cJSON_IsFalse(retrieved);

    // step 5: Validate
    validation_score = (retrieved != (cJSON *)0) + (int)added + (int)retrieved_is_false;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}