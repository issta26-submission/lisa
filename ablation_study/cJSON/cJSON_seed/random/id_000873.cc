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
//<ID> 873
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[{\"a\":1}, {\"a\":1}, {\"a\":2}]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *copy = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    cJSON_bool added_extracted = 0;
    cJSON_bool added_copy = 0;
    cJSON_bool equal_flags = 0;
    int size_before = 0;
    int size_after = 0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);

    // step 3: Operate on parsed array (measure size and detach an element)
    size_before = cJSON_GetArraySize(parsed);
    detached = cJSON_DetachItemFromArray(parsed, 1);
    size_after = cJSON_GetArraySize(parsed);

    // step 4: Duplicate detached item and configure root object
    copy = cJSON_Duplicate(detached, (cJSON_bool)1);
    root = cJSON_CreateObject();
    added_extracted = cJSON_AddItemToObject(root, "extracted", detached);
    added_copy = cJSON_AddItemToObject(root, "copy", copy);

    // step 5: Validate results (compare detached with its duplicate and aggregate checks)
    equal_flags = cJSON_Compare(detached, copy, (cJSON_bool)1);
    validation_score = size_before + size_after + (int)added_extracted + (int)added_copy + (int)equal_flags;
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}