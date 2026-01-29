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
//<ID> 315
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"status\":true,\"info\":{\"version\":\"1.0\"}}";
    cJSON *root = cJSON_Parse(json);

    // step 2: Configure
    cJSON *status_item = cJSON_GetObjectItem(root, "status");
    cJSON *info_item = cJSON_GetObjectItem(root, "info");
    cJSON *new_status = cJSON_CreateFalse();
    cJSON_ReplaceItemViaPointer(root, status_item, new_status);
    cJSON *new_info = cJSON_CreateObject();
    cJSON_AddStringToObject(new_info, "version", "2.0");
    cJSON_ReplaceItemViaPointer(root, info_item, new_info);

    // step 3: Operate and Validate
    cJSON *status_after = cJSON_GetObjectItem(root, "status");
    cJSON_bool status_is_bool = cJSON_IsBool(status_after);
    cJSON *info_after = cJSON_GetObjectItem(root, "info");
    cJSON_bool info_is_obj = cJSON_IsObject(info_after);
    double flags = (double)status_is_bool + (double)(info_is_obj * 2);
    cJSON_AddNumberToObject(root, "flags", flags);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}