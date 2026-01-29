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
//<ID> 832
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
    cJSON *array = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *false_in_array = (cJSON *)0;
    cJSON *ref_item = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON *retrieved_list = (cJSON *)0;
    cJSON *retrieved_list_elem = (cJSON *)0;
    cJSON_bool added_owner = 0;
    cJSON_bool added_list = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool is_array_flag = 0;
    cJSON_bool is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    owner = cJSON_CreateObject();
    array = cJSON_CreateArray();
    false_item = cJSON_CreateFalse();
    false_in_array = cJSON_CreateFalse();

    // step 3: Configure
    added_owner = cJSON_AddItemToObject(owner, "flag", false_item);
    added_list = cJSON_AddItemToArray(array, false_in_array);
    cJSON_AddItemToObject(root, "owner", owner);
    cJSON_AddItemToObject(root, "list", array);
    ref_item = cJSON_CreateObjectReference(false_item);
    added_ref = cJSON_AddItemReferenceToObject(root, "flag_ref", ref_item);

    // step 4: Operate
    retrieved_ref = cJSON_GetObjectItem(root, "flag_ref");
    retrieved_list = cJSON_GetObjectItem(root, "list");
    retrieved_list_elem = cJSON_GetArrayItem(retrieved_list, 0);
    is_array_flag = cJSON_IsArray(retrieved_list);
    is_false_flag = cJSON_IsFalse(retrieved_list_elem);

    // step 5: Validate
    validation_score = (int)added_owner + (int)added_list + (int)added_ref + (retrieved_ref != (cJSON *)0) + (retrieved_list != (cJSON *)0) + (retrieved_list_elem != (cJSON *)0) + (int)is_array_flag + (int)is_false_flag;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}