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
//<ID> 1156
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"status\":\"ok\",\"active\":true}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *created_obj = cJSON_CreateObject();
    cJSON *status_str = cJSON_CreateString("ok");
    cJSON *active_bool = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(created_obj, "status", status_str);
    cJSON_AddItemToObject(created_obj, "active", active_bool);
    cJSON *root = cJSON_CreateObject();
    cJSON *source_label = cJSON_CreateString("constructed");
    cJSON_AddItemToObject(root, "source", source_label);

    // step 3: Operate & Validate
    cJSON_bool equal = cJSON_Compare(parsed, created_obj, 1);
    cJSON *match_flag = cJSON_CreateBool(equal);
    cJSON_AddItemToObject(root, "match", match_flag);
    cJSON *parsed_status = cJSON_GetObjectItem(parsed, "status");
    const char *status_value = cJSON_GetStringValue(parsed_status);
    cJSON *parsed_status_copy = cJSON_CreateString(status_value);
    cJSON_AddItemToObject(root, "parsed_status", parsed_status_copy);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    cJSON_Delete(created_obj);
    // API sequence test completed successfully
    return 66;
}