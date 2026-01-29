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
//<ID> 834
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
    cJSON *owner_ref = (cJSON *)0;
    cJSON *retrieved_owner_ref = (cJSON *)0;
    cJSON *retrieved_arr = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added_owner_ref = 0;
    cJSON_bool added_arr = 0;
    cJSON_bool is_array_flag = 0;
    cJSON_bool is_false_flag = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    owner = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(owner, "flag", false_item);
    owner_ref = cJSON_CreateObjectReference(owner);
    added_owner_ref = cJSON_AddItemReferenceToObject(root, "owner_ref", owner_ref);
    added_arr = cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("elem"));

    // step 4: Operate
    retrieved_owner_ref = cJSON_GetObjectItem(root, "owner_ref");
    retrieved_arr = cJSON_GetObjectItem(root, "list");
    is_array_flag = cJSON_IsArray(retrieved_arr);
    is_false_flag = cJSON_IsFalse(cJSON_GetObjectItem(owner, "flag"));

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)added_owner_ref + (int)added_arr + (retrieved_owner_ref != (cJSON *)0) + (retrieved_arr != (cJSON *)0) + (int)is_array_flag + (int)is_false_flag + (printed != (char *)0);
    (void)validation_score;
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(owner);

    // API sequence test completed successfully
    return 66;
}