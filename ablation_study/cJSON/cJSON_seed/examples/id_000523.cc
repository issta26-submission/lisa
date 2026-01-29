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
//<ID> 523
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *enabled = cJSON_CreateTrue();
    cJSON_bool added_cs = cJSON_AddItemToObjectCS(root, "enabled", enabled);

    // step 2: Configure
    cJSON_AddStringToObject(config, "name", "demo-name");
    cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *inner = cJSON_AddObjectToObject(config, "inner");
    cJSON_AddStringToObject(inner, "note", "inner-note");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_config, "name");
    char *name_str = cJSON_GetStringValue(parsed_name);
    cJSON *parsed_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON_bool is_true = cJSON_IsTrue(parsed_enabled);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;enabled=%d;equal=%d", name_str, (int)is_true, (int)equal);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}