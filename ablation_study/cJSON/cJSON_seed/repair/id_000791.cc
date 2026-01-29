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
//<ID> 791
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON *entry0 = cJSON_CreateObject();
    cJSON_AddStringToObject(entry0, "name", "entry_zero");
    cJSON_AddTrueToObject(entry0, "enabled");
    cJSON_AddItemToArray(items, entry0);
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddStringToObject(entry1, "name", "entry_one");
    cJSON_AddBoolToObject(entry1, "enabled", 0);
    cJSON_AddItemToArray(items, entry1);
    cJSON_AddStringToObject(meta, "creator", "tester");

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_item);
    (void)first_is_invalid;
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}