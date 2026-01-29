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
//<ID> 1518
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child_false = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON_bool added_flag = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_false = cJSON_CreateFalse();

    // step 3: Configure
    added_flag = cJSON_AddItemToObject(root, "flag_false", child_false);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "flag_false");
    is_false = cJSON_IsFalse(retrieved);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)added_flag + (int)(retrieved != (cJSON *)0) + (int)is_false;

    // step 6: Cleanup
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved;
    (void)added_flag;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}