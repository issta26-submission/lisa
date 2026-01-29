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
//<ID> 794
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *label_item = cJSON_CreateString("sensor");
    cJSON *id_item = cJSON_CreateString("alpha-01");
    cJSON *meta = cJSON_CreateObject();
    cJSON *version_item = cJSON_CreateString("v1");

    // step 2: Configure
    cJSON_AddItemToObject(root, "label", label_item);
    cJSON_AddItemToObject(root, "id", id_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "version", version_item);
    cJSON_AddNumberToObject(root, "value", 42.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_label = cJSON_GetObjectItemCaseSensitive(parsed, "label");
    char *label_val = cJSON_GetStringValue(parsed_label);
    cJSON *parsed_id = cJSON_GetObjectItem(parsed, "id");
    char *id_val = cJSON_GetStringValue(parsed_id);
    cJSON *new_id_copy = cJSON_CreateString(id_val);
    cJSON_AddItemToObject(parsed, "id_copy", new_id_copy);
    cJSON *fetched_copy = cJSON_GetObjectItemCaseSensitive(parsed, "id_copy");
    char *copy_val = cJSON_GetStringValue(fetched_copy);
    (void)label_val;
    (void)id_val;
    (void)copy_val;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}