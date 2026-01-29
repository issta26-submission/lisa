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
//<ID> 1490
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
    cJSON *null_item = (cJSON *)0;
    cJSON *retrieved_false = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_false = 0;
    cJSON_bool added_null = 0;
    cJSON_bool is_null_flag = 0;
    cJSON_bool is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    null_item = cJSON_CreateNull();

    // step 3: Configure
    added_false = cJSON_AddItemToObject(root, "feature_enabled", false_item);
    added_null = cJSON_AddItemToObject(root, "optional_value", null_item);

    // step 4: Operate
    retrieved_false = cJSON_GetObjectItem(root, "feature_enabled");
    retrieved_null = cJSON_GetObjectItem(root, "optional_value");
    is_false_flag = cJSON_IsFalse(retrieved_false);
    is_null_flag = cJSON_IsNull(retrieved_null);
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)added_false + (int)added_null + (int)is_false_flag + (int)is_null_flag + (int)(printed != (char *)0);

    // step 5: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_false;
    (void)retrieved_null;
    // API sequence test completed successfully
    return 66;
}