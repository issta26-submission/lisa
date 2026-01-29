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
//<ID> 399
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
    cJSON_AddItemToObject(root, "child", child);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(root, "value", num);
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_AddItemToObject(root, "child_ref", child_ref);

    // step 2: Configure
    cJSON *value_item = cJSON_GetObjectItemCaseSensitive(root, "value");
    double value_number = cJSON_GetNumberValue(value_item);
    cJSON *doubled = cJSON_CreateNumber(value_number * 2.0);
    cJSON_AddItemToObject(root, "value_doubled", doubled);
    char *snapshot_before = cJSON_PrintBuffered(root, 128, 1);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "child");
    cJSON *extra = cJSON_CreateNumber(7.25);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 4: Validate and Cleanup
    char *snapshot_after = cJSON_PrintBuffered(root, 64, 0);
    cJSON_free(snapshot_before);
    cJSON_free(snapshot_after);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}