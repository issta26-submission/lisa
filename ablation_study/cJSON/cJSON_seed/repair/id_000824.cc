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
//<ID> 824
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *items = cJSON_AddArrayToObject(data, "items");
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    const char *json_text = "{\"settings\":{\"threshold\":42.5},\"remove_me\":\"bye\",\"name\":\"hello\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *name_ptr = cJSON_GetObjectItemCaseSensitive(parsed, "name");

    // step 3: Operate and Validate
    cJSON *settings = cJSON_GetObjectItemCaseSensitive(parsed, "settings");
    cJSON *threshold_item = cJSON_GetObjectItemCaseSensitive(settings, "threshold");
    double threshold_value = cJSON_GetNumberValue(threshold_item);
    cJSON_AddNumberToObject(data, "threshold_from_parsed", threshold_value);
    cJSON *detached_by_name = cJSON_DetachItemFromObject(parsed, "remove_me");
    cJSON_bool name_is_string = cJSON_IsString(name_ptr);
    cJSON *detached_by_ptr = cJSON_DetachItemViaPointer(parsed, name_ptr);
    cJSON_AddItemToObject(root, "imported_removed", detached_by_name);
    cJSON_AddItemToObject(root, "imported_name", detached_by_ptr);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);
    (void)name_is_string;

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}