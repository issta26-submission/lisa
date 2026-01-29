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
//<ID> 798
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *id_item = cJSON_CreateString("device-42");
    cJSON *status_item = cJSON_CreateString("operational");
    cJSON_AddItemToObject(root, "id", id_item);
    cJSON_AddItemToObject(root, "status", status_item);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "location", "rack-7");
    cJSON_AddNumberToObject(meta, "uptime_hours", 1234.5);

    // step 3: Operate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);

    cJSON *parsed_id_cs = cJSON_GetObjectItemCaseSensitive(parsed, "id");
    char *extracted_id = cJSON_GetStringValue(parsed_id_cs);
    cJSON *created_copy = cJSON_CreateString(extracted_id);
    cJSON_AddItemToObject(parsed, "id_copy", created_copy);

    cJSON *parsed_status = cJSON_GetObjectItem(parsed, "status");
    char *extracted_status = cJSON_GetStringValue(parsed_status);
    (void)extracted_status;
    int id_length = (int)strlen(extracted_id);
    (void)id_length;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}