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
    cJSON *info = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("device-xyz");
    cJSON *model_item = cJSON_CreateString("x100");
    cJSON_AddItemToObject(info, "name", name_item);
    cJSON_AddItemToObject(info, "model", model_item);
    cJSON_AddItemToObject(root, "info", info);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON *owner_item = cJSON_CreateString("team-a");
    cJSON_AddItemToObject(meta, "owner", owner_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_info = cJSON_GetObjectItemCaseSensitive(parsed, "info");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_info, "name");
    char *extracted_name = cJSON_GetStringValue(parsed_name);
    cJSON *copied_name_item = cJSON_CreateString(extracted_name);
    cJSON_AddItemToObject(parsed, "copied_name", copied_name_item);
    cJSON *retrieved = cJSON_GetObjectItem(parsed, "copied_name");
    char *retrieved_value = cJSON_GetStringValue(retrieved);
    (void)retrieved_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}