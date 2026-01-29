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
//<ID> 838
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *owner = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool added_owner = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool is_array_flag = 0;
    cJSON_bool is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    owner = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_owner = cJSON_AddItemToObject(owner, "flag", false_item);
    ref_item = cJSON_CreateObjectReference(false_item);
    added_ref = cJSON_AddItemReferenceToObject(root, "flag_ref", ref_item);

    // step 4: Operate
    retrieved_ref = cJSON_GetObjectItem(root, "flag_ref");
    retrieved_flag = cJSON_GetObjectItem(owner, "flag");
    is_array_flag = cJSON_IsArray(retrieved_ref);
    is_false_flag = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate
    validation_score = (retrieved_ref != (cJSON *)0) + (retrieved_flag != (cJSON *)0) + (int)is_array_flag + (int)is_false_flag + (int)added_owner + (int)added_ref;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(owner);

    // API sequence test completed successfully
    return 66;
}