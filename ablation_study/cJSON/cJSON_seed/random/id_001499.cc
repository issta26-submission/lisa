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
//<ID> 1499
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
    cJSON *child_null = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_null = 0;
    cJSON_bool is_null = 0;
    cJSON_bool is_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    child_false = cJSON_CreateFalse();
    child_null = cJSON_CreateNull();

    // step 3: Configure
    added_flag = cJSON_AddItemToObject(root, "flag_false", child_false);
    added_null = cJSON_AddItemToObject(root, "maybe_null", child_null);

    // step 4: Operate
    printed = cJSON_PrintUnformatted(root);
    retrieved_null = cJSON_GetObjectItem(root, "maybe_null");
    is_null = cJSON_IsNull(retrieved_null);
    is_false = cJSON_IsFalse(cJSON_GetObjectItem(root, "flag_false"));

    // step 5: Validate
    validation_score = (int)(printed != (char *)0) + (int)added_flag + (int)added_null + (int)is_null + (int)is_false;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_null;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}