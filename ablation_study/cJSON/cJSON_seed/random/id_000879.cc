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
//<ID> 879
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[\"alpha\",\"beta\",\"gamma\",\"delta\"]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *dup_detached = (cJSON *)0;
    cJSON_bool compare_equal = (cJSON_bool)0;
    cJSON_bool added_remaining = (cJSON_bool)0;
    cJSON_bool added_extracted = (cJSON_bool)0;
    int initial_size = 0;
    int after_detach_size = 0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array (measure size and detach element)
    initial_size = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 2);
    after_detach_size = cJSON_GetArraySize(parsed);

    // step 4: Configure object and add items
    root = cJSON_CreateObject();
    added_remaining = cJSON_AddItemToObject(root, "remaining", parsed);
    added_extracted = cJSON_AddItemToObject(root, "extracted", detached);

    // step 5: Validate by duplicating detached and comparing
    dup_detached = cJSON_Duplicate(detached, (cJSON_bool)1);
    compare_equal = cJSON_Compare(detached, dup_detached, (cJSON_bool)1);
    validation_score = initial_size + after_detach_size + (int)added_remaining + (int)added_extracted + (int)compare_equal + (dup_detached != (cJSON *)0) + (root != (cJSON *)0);

    // step 6: Cleanup
    cJSON_Delete(dup_detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}