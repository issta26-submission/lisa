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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_CreateObject();
    cJSON *count = cJSON_CreateNumber(42.0);
    cJSON *status = cJSON_CreateString("ok");
    cJSON *flag = cJSON_CreateTrue();
    cJSON *arr = cJSON_CreateArray();
    cJSON *elem = cJSON_CreateString("elem1");

    // step 2: Configure
    cJSON_AddItemToArray(arr, elem);
    cJSON_AddItemToObjectCS(cfg, "opts", arr);
    cJSON_AddItemToObjectCS(root, "config", cfg);
    cJSON_AddItemToObjectCS(root, "count", count);
    cJSON_AddItemToObjectCS(root, "status", status);
    cJSON_AddItemToObjectCS(root, "flag", flag);

    // step 3: Operate & Validate
    cJSON_bool has_cfg = cJSON_HasObjectItem(root, "config");
    cJSON *has_cfg_item = cJSON_CreateBool(has_cfg);
    cJSON_AddItemToObjectCS(root, "has_config", has_cfg_item);
    cJSON *status_ptr = cJSON_GetObjectItem(root, "status");
    const char *status_text = cJSON_GetStringValue(status_ptr);
    cJSON *status_copy = cJSON_CreateString(status_text);
    cJSON_AddItemToObjectCS(root, "status_copy", status_copy);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "flag");
    cJSON_bool has_flag_after = cJSON_HasObjectItem(root, "flag");
    cJSON_AddItemToObjectCS(root, "has_flag_after_delete", cJSON_CreateBool(has_flag_after));
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}