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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"original\",\"count\":3,\"flag\":false}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);
    cJSON *orig_name_item = cJSON_GetObjectItem(root, "name");
    const char *orig_name = cJSON_GetStringValue(orig_name_item);

    // step 2: Configure
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_bool replaced_name = cJSON_ReplaceItemInObject(root, "name", new_name);
    cJSON *count_item_before = cJSON_GetObjectItem(root, "count");
    double count_before = cJSON_GetNumberValue(count_item_before);
    cJSON *new_count = cJSON_CreateNumber(count_before + 1.0);
    cJSON_bool replaced_count = cJSON_ReplaceItemInObject(root, "count", new_count);

    // step 3: Operate and Validate
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    const char *name_val = cJSON_GetStringValue(name_item);
    cJSON *count_item_after = cJSON_GetObjectItem(root, "count");
    double count_after = cJSON_GetNumberValue(count_item_after);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    cJSON *flag_after = cJSON_GetObjectItem(root, "flag");
    (void)orig_name;
    (void)name_val;
    (void)count_before;
    (void)count_after;
    (void)replaced_name;
    (void)replaced_count;
    (void)flag_after;

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}