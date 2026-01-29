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
//<ID> 872
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json = "[{\"a\":1},{\"a\":1},{\"a\":2}]";
    cJSON *parsed = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first = (cJSON *)0;
    cJSON *root = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool cmp_equal = 0;
    cJSON_bool added_parsed = 0;
    cJSON_bool added_detached = 0;
    int validation_score = 0;

    // step 2: Initialize / Parse input
    parsed = cJSON_Parse(json);
    size_before = cJSON_GetArraySize(parsed);

    // step 3: Operate on parsed array (detach an element)
    detached = cJSON_DetachItemFromArray(parsed, 1);
    first = cJSON_GetArrayItem(parsed, 0);
    size_after = cJSON_GetArraySize(parsed);

    // step 4: Configure object and add items
    root = cJSON_CreateObject();
    added_parsed = cJSON_AddItemToObject(root, "remaining", parsed);
    added_detached = cJSON_AddItemToObject(root, "extracted", detached);

    // step 5: Validate results (compare detached with first and aggregate checks)
    cmp_equal = cJSON_Compare(first, detached, (cJSON_bool)1);
    validation_score = (int)cmp_equal + (int)added_parsed + (int)added_detached + (size_before == 3) + (size_after == 2) + (root != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}