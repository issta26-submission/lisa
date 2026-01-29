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
//<ID> 558
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
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(child, "value", num);
    cJSON_AddBoolToObject(child, "flag", 1);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", nul);
    cJSON_AddStringToObject(child, "name", "example");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_child = cJSON_GetObjectItem(parsed, "child");
    cJSON *parsed_value = cJSON_GetObjectItem(parsed_child, "value");
    double value = cJSON_GetNumberValue(parsed_value);
    cJSON *parsed_flag = cJSON_GetObjectItem(parsed_child, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(parsed_flag);
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_child, "name");
    char *name_str = (char *)cJSON_GetStringValue(parsed_name);
    cJSON *parsed_nothing = cJSON_GetObjectItem(parsed, "nothing");
    cJSON_bool is_null = cJSON_IsNull(parsed_nothing);
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;val=%.2f;flag=%d;name=%s;null=%d",
            version ? version : "null",
            value,
            (int)flag_true,
            name_str ? name_str : "null",
            (int)is_null);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}