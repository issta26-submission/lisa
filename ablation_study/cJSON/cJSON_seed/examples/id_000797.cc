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
//<ID> 797
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("device-42");
    cJSON *meta = cJSON_CreateObject();
    cJSON *id = cJSON_CreateString("abc123");
    cJSON_AddItemToObject(meta, "id", id);
    cJSON_AddItemToObject(root, "name", name);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    char *name_value = cJSON_GetStringValue(parsed_name_item);
    cJSON *parsed_meta_item = cJSON_GetObjectItem(parsed, "meta");
    cJSON *parsed_meta_id_item = cJSON_GetObjectItemCaseSensitive(parsed_meta_item, "id");
    char *id_value = cJSON_GetStringValue(parsed_meta_id_item);
    cJSON *name_copy = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(parsed, "name_copy", name_copy);
    cJSON *id_copy = cJSON_CreateString(id_value);
    cJSON_AddItemToObject(parsed, "meta_id_copy", id_copy);
    char *final_snapshot = cJSON_Print(parsed);
    (void)final_snapshot;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_free(final_snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}