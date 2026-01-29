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
//<ID> 793
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Sensor-01");
    cJSON_AddItemToObject(meta, "Name", name);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON *type = cJSON_CreateString("temperature");
    cJSON_AddItemToObject(root, "type", type);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_meta, "name");
    char *name_value = cJSON_GetStringValue(parsed_name);
    cJSON *dup_name = cJSON_CreateString(name_value);
    cJSON_AddItemToObject(parsed, "name_copy", dup_name);
    cJSON *name_copy_item = cJSON_GetObjectItem(parsed, "name_copy");
    char *copy_value = cJSON_GetStringValue(name_copy_item);
    (void)name_value;
    (void)copy_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}