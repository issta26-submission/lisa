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
//<ID> 836
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
    cJSON *arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *ref_to_false = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON_bool added_list = 0;
    cJSON_bool added_flag = 0;
    cJSON_bool added_owner = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool is_list_array = 0;
    cJSON_bool is_flag_false = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    owner = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_list = cJSON_AddItemToObject(owner, "list", arr);
    added_flag = cJSON_AddItemToObject(owner, "flag", false_item);
    added_owner = cJSON_AddItemToObject(root, "owner", owner);
    ref_to_false = cJSON_CreateObjectReference(false_item);
    added_ref = cJSON_AddItemReferenceToObject(root, "flag_ref", ref_to_false);

    // step 4: Operate
    retrieved_list = cJSON_GetObjectItem(owner, "list");
    retrieved_flag = cJSON_GetObjectItem(owner, "flag");
    is_list_array = cJSON_IsArray(retrieved_list);
    is_flag_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate
    validation_score = (int)added_list + (int)added_flag + (int)added_owner + (int)added_ref + (int)is_list_array + (int)is_flag_false + (retrieved_list != (cJSON *)0) + (retrieved_flag != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}