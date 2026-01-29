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
//<ID> 671
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);
    cJSON *status = cJSON_CreateTrue();
    cJSON_AddItemToObject(config, "status", status);
    cJSON *list = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", list);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, s1);
    cJSON_AddItemToArray(list, s2);

    // step 2: Configure
    cJSON *status_before = cJSON_GetObjectItem(config, "status");
    cJSON_bool was_bool = cJSON_IsBool(status_before);
    cJSON *replacement = cJSON_CreateString("ok");
    cJSON_ReplaceItemInObject(config, "status", replacement);

    // step 3: Operate
    cJSON *status_after = cJSON_GetObjectItem(config, "status");
    cJSON_bool now_string = cJSON_IsString(status_after);
    int list_size = cJSON_GetArraySize(list);
    cJSON_AddItemToObject(root, "list_size", cJSON_CreateNumber((double)list_size));
    cJSON_AddItemToObject(root, "was_bool", cJSON_CreateBool(was_bool));
    cJSON_AddItemToObject(root, "is_string", cJSON_CreateBool(now_string));

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}