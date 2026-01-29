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
//<ID> 516
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
    cJSON *config = cJSON_CreateObject();
    cJSON *meta_cs = cJSON_CreateObject();
    cJSON *s1 = cJSON_CreateString("value1");
    cJSON *s2 = cJSON_CreateString("value2");
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddItemToObjectCS(root, "meta", meta_cs);
    cJSON_AddItemToObject(config, "key1", s1);
    cJSON_AddItemToObject(config, "key2", s2);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "count", 2.0);
    cJSON_AddStringToObject(meta_cs, "status", "ok");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_key1 = cJSON_GetObjectItem(parsed_config, "key1");
    const char *key1_str = cJSON_GetStringValue(parsed_key1);
    char status_buf[128];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "ver=%s;key1=%s;equal=%d", version ? version : "null", key1_str ? key1_str : "null", (int)equal);
    char *parsed_snapshot = cJSON_Print(parsed);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(parsed_snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}