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
//<ID> 1515
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
    cJSON *true_item = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_true = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added_false = cJSON_AddItemToObject(root, "false_key", false_item);
    added_true = cJSON_AddItemToObject(root, "true_key", true_item);

    // step 4: Operate
    retrieved_false = cJSON_GetObjectItem(root, "false_key");
    is_false = cJSON_IsFalse(retrieved_false);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0) + (int)added_false + (int)added_true + (int)(retrieved_false != (cJSON *)0) + (int)is_false + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_false;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}