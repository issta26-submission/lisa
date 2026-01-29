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
//<ID> 398
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(child, "answer", answer);
    cJSON_AddItemToObject(root, "child", child);
    cJSON *pi = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(root, "pi", pi);

    // step 2: Configure
    // Create a duplicate of 'child' and add it as "child_ref" to avoid double-free issues
    cJSON *child_dup = cJSON_Duplicate(child, 1);
    cJSON_AddItemToObject(root, "child_ref", child_dup);
    char *snapshot_before = cJSON_PrintBuffered(root, 256, 1);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "pi");
    cJSON *hundred = cJSON_CreateNumber(100.0);
    cJSON_AddItemToObject(root, "hundred", hundred);
    cJSON *child_lookup = cJSON_GetObjectItemCaseSensitive(root, "child");
    cJSON *answer_lookup = cJSON_GetObjectItemCaseSensitive(child_lookup, "answer");
    double answer_value = cJSON_GetNumberValue(answer_lookup);
    cJSON *hundred_lookup = cJSON_GetObjectItemCaseSensitive(root, "hundred");
    double hundred_value = cJSON_GetNumberValue(hundred_lookup);
    double total = answer_value + hundred_value;
    cJSON *sum_item = cJSON_CreateNumber(total);
    cJSON_AddItemToObject(root, "sum", sum_item);

    // step 4: Validate and Cleanup
    char *snapshot_after = cJSON_PrintBuffered(root, 512, 0);
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    return 66;
}