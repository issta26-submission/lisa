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
//<ID> 1686
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"enabled\":true,\"status\":\"ok\"}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *status_str = cJSON_CreateString("ok");
    cJSON_bool added_status = cJSON_AddItemToObject(root, "status", status_str);
    cJSON *enabled_true = cJSON_CreateTrue();
    cJSON_bool added_enabled = cJSON_AddItemToObject(root, "enabled", enabled_true);

    // step 3: Operate and Validate
    cJSON *p_enabled = cJSON_GetObjectItem(parsed, "enabled");
    cJSON *p_status = cJSON_GetObjectItem(parsed, "status");
    cJSON_bool parsed_enabled_is_true = cJSON_IsTrue(p_enabled);
    cJSON *r_status = cJSON_GetObjectItem(root, "status");
    cJSON_bool r_status_invalid = cJSON_IsInvalid(r_status);
    char *printed_root = cJSON_Print(root);
    char *printed_parsed = cJSON_Print(parsed);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed_root ? printed_root[0] : 'X';
    buffer[1] = printed_parsed ? printed_parsed[0] : 'Y';
    buffer[2] = (char)('0' + (int)parsed_enabled_is_true);
    buffer[3] = (char)('0' + (int)r_status_invalid);
    (void)added_status;
    (void)added_enabled;
    (void)p_status;
    (void)r_status;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_parsed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}