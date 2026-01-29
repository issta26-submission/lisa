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
//<ID> 852
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *ref = cJSON_CreateStringReference("example_reference");
    cJSON_AddItemToObject(root, "ref", ref);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *count = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(root, "count", count);

    // step 2: Configure
    cJSON *new_count = cJSON_CreateNumber(42.0);
    cJSON_bool replace_ok = cJSON_ReplaceItemInObject(root, "count", new_count);
    const char *ref_val = cJSON_GetStringValue(cJSON_GetObjectItem(root, "ref"));
    double count_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "count"));
    (void)replace_ok;
    (void)ref_val;
    (void)count_val;

    // step 3: Operate
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "ref");
    const cJSON *enabled_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    cJSON_bool is_enabled = cJSON_IsTrue(enabled_item);
    (void)is_enabled;
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}