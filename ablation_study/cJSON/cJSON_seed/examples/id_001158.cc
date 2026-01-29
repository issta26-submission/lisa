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
//<ID> 1158
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
    cJSON *root = cJSON_CreateObject();
    cJSON *status_str = cJSON_CreateString("ok");
    cJSON *active_bool = cJSON_CreateBool(1);

    // step 2: Configure
    cJSON_AddItemToObject(root, "status", status_str);
    cJSON_AddItemToObject(root, "active", active_bool);

    // step 3: Operate & Validate
    cJSON *p_status = cJSON_GetObjectItem(parsed, "status");
    char *p_status_val = cJSON_GetStringValue(p_status);
    cJSON_bool are_equal = cJSON_Compare(parsed, root, 1);
    cJSON *identical_flag = cJSON_CreateBool(are_equal);
    cJSON_AddItemToObject(root, "identical", identical_flag);
    cJSON *dup_status = cJSON_CreateString(p_status_val);
    cJSON_AddItemToObject(root, "parsed_status", dup_status);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}