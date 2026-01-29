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
//<ID> 839
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
    cJSON *arr = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    cJSON_bool added_owner = 0;
    cJSON_bool added_owner_to_root = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool added_array = 0;
    cJSON_bool is_false_flag = 0;
    cJSON_bool is_array_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    owner = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    arr = cJSON_CreateArray();

    // step 3: Configure
    added_owner = cJSON_AddItemToObject(owner, "status", false_item);
    added_owner_to_root = cJSON_AddItemToObject(root, "owner", owner);
    added_ref = cJSON_AddItemReferenceToObject(root, "status_ref", false_item);
    added_array = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    retrieved_ref = cJSON_GetObjectItem(root, "status_ref");
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    is_false_flag = cJSON_IsFalse(retrieved_ref);
    is_array_flag = cJSON_IsArray(retrieved_arr);

    // step 5: Validate
    validation_score = (retrieved_ref != (cJSON *)0) + (retrieved_arr != (cJSON *)0) + (int)is_false_flag + (int)is_array_flag + (int)added_owner + (int)added_owner_to_root + (int)added_ref + (int)added_array;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}