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
//<ID> 529
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
    cJSON *meta_cs = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta_cs", meta_cs);

    // step 2: Configure
    cJSON *flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "enabled", flag);
    cJSON *name = cJSON_CreateString("sample-name");
    cJSON_AddItemToObject(config, "name", name);
    cJSON *meta_note = cJSON_CreateString("metadata");
    cJSON_AddItemToObject(meta_cs, "note", meta_note);

    // step 3: Operate & Validate
    char *name_value = cJSON_GetStringValue(name);
    cJSON_bool flag_is_true = cJSON_IsTrue(flag);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_config = cJSON_GetObjectItem(parsed, "config");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_config, "name");
    char *parsed_name_value = cJSON_GetStringValue(parsed_name);
    cJSON_bool equal = cJSON_Compare(root, parsed, 1);

    // step 4: Cleanup
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "name=%s;parsed_name=%s;flag_is_true=%d;equal=%d",
            name_value ? name_value : "null",
            parsed_name_value ? parsed_name_value : "null",
            (int)flag_is_true,
            (int)equal);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}