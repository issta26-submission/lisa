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
//<ID> 512
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON_AddItemToObjectCS(root, "meta_cs", meta);

    // step 2: Configure
    cJSON_AddStringToObject(root, "name", "cjson_demo");
    cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *parsed_meta = cJSON_GetObjectItem(parsed, "meta_cs");
    cJSON *parsed_status = cJSON_GetObjectItem(parsed_meta, "status");
    const char *status_str = cJSON_GetStringValue(parsed_status);
    sprintf(status_buf, "ver=%s;status=%s;equal=%d", version ? version : "unknown", status_str ? status_str : "null", (int)equal);
    char *parsed_snapshot = cJSON_Print(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(parsed_snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}