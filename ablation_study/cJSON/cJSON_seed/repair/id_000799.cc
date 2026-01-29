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
//<ID> 799
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
    cJSON *entry0 = cJSON_CreateObject();
    cJSON_AddTrueToObject(entry0, "enabled");
    cJSON_AddNumberToObject(entry0, "id", 42.0);
    cJSON_AddItemToArray(items, entry0);
    cJSON *entry1 = cJSON_CreateObject();
    cJSON_AddTrueToObject(entry1, "enabled");
    cJSON_AddNumberToObject(entry1, "id", 84.0);
    cJSON_AddItemToArray(items, entry1);

    // step 2: Configure
    const char *json_text = "[{\"name\":\"alpha\"},{\"name\":\"beta\"}]";
    cJSON *parsed = cJSON_Parse(json_text);

    // step 3: Operate and Validate
    cJSON *second_parsed_item = cJSON_GetArrayItem(parsed, 1);
    cJSON_bool parsed_item_invalid = cJSON_IsInvalid(second_parsed_item);
    char *snapshot_root = cJSON_PrintUnformatted(root);
    char *snapshot_parsed = cJSON_PrintUnformatted(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot_root);
    cJSON_free(snapshot_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}