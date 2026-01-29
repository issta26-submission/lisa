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
//<ID> 881
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *value = cJSON_CreateNumber(3.1415);
    cJSON *enabled = cJSON_CreateBool(1);
    cJSON *meta = cJSON_CreateObject();
    cJSON *idnum = cJSON_CreateNumber(7);

    // step 2: Configure
    cJSON_AddItemToObjectCS(root, "value", value);
    cJSON_AddItemToObject(root, "enabled", enabled);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObjectCS(meta, "id", idnum);

    // step 3: Operate & Validate
    cJSON *got_value = cJSON_GetObjectItem(root, "value");
    double numeric = cJSON_GetNumberValue(got_value);
    cJSON *double_value = cJSON_CreateNumber(numeric * 2.0);
    cJSON_AddItemToObject(root, "double_value", double_value);
    cJSON *got_enabled = cJSON_GetObjectItem(root, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON *enabled_flag = cJSON_CreateNumber((double)enabled_is_bool);
    cJSON_AddItemToObjectCS(root, "enabled_is_bool", enabled_flag);
    char *snapshot = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}