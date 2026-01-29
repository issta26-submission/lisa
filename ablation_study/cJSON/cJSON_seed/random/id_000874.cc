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
//<ID> 874
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON *dup_detached = (cJSON *)0;
    cJSON_bool compare_equal = (cJSON_bool)0;
    cJSON_bool added_remaining = (cJSON_bool)0;
    cJSON_bool added_extracted = (cJSON_bool)0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize / Parse input array
    parsed = cJSON_Parse("[10,20,30,40]");

    // step 3: Operate on parsed array (measure size and detach an element)
    size_before = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 2);
    size_after = cJSON_GetArraySize(parsed);

    // step 4: Configure an object and attach array and detached item
    root = cJSON_CreateObject();
    added_remaining = cJSON_AddItemToObject(root, "remaining", parsed);
    added_extracted = cJSON_AddItemToObject(root, "extracted", detached);

    // step 5: Validate by duplicating detached item and comparing
    dup_detached = cJSON_Duplicate(detached, (cJSON_bool)1);
    compare_equal = cJSON_Compare(detached, dup_detached, (cJSON_bool)1);
    validation_score = (int)compare_equal + (int)added_remaining + (int)added_extracted + (int)(size_before > size_after);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_detached);

    // API sequence test completed successfully
    return 66;
}