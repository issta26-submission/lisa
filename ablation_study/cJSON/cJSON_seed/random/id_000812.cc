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
//<ID> 812
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item_null = (cJSON *)0;
    cJSON *item_false = (cJSON *)0;
    cJSON *retrieved_null = (cJSON *)0;
    cJSON *retrieved_ref = (cJSON *)0;
    cJSON_bool added_null = 0;
    cJSON_bool added_ref = 0;
    cJSON_bool is_null_on_retrieved = 0;
    cJSON_bool is_null_on_ref = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item_null = cJSON_CreateNull();
    item_false = cJSON_CreateFalse();

    // step 3: Configure
    added_null = cJSON_AddItemToObject(root, "nothing", item_null);
    added_ref = cJSON_AddItemReferenceToObject(root, "flag_ref", item_false);

    // step 4: Operate
    retrieved_null = cJSON_GetObjectItem(root, "nothing");
    retrieved_ref = cJSON_GetObjectItem(root, "flag_ref");
    is_null_on_retrieved = cJSON_IsNull(retrieved_null);
    is_null_on_ref = cJSON_IsNull(retrieved_ref);

    // step 5: Validate
    validation_score = (retrieved_null != (cJSON *)0) + (int)is_null_on_retrieved + (retrieved_ref != (cJSON *)0) + (int)is_null_on_ref + (int)added_null + (int)added_ref;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(item_false);

    // API sequence test completed successfully
    return 66;
}