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
//<ID> 813
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
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_null = 0;
    cJSON_bool is_null = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    null_item = cJSON_CreateNull();

    // step 3: Configure
    added_flag = cJSON_AddItemToObject(root, "flag", false_item);
    added_ref = cJSON_AddItemReferenceToObject(root, "flag_ref", false_item);
    added_null = cJSON_AddItemToObject(root, "nothing", null_item);

    // step 4: Operate
    retrieved_flag = cJSON_GetObjectItem(root, "flag");
    retrieved_ref = cJSON_GetObjectItem(root, "flag_ref");
    retrieved_null = cJSON_GetObjectItem(root, "nothing");
    is_null = cJSON_IsNull(retrieved_null);

    // step 5: Validate
    validation_score = (int)added_flag + (int)added_ref + (int)added_null + (retrieved_flag != (cJSON *)0) + (retrieved_ref != (cJSON *)0) + (int)is_null;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}