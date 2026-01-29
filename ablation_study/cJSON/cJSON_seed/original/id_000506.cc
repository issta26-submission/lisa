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
//<ID> 506
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("Eve");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *raw_item = cJSON_CreateRaw("{\"meta\":true}");
    cJSON_AddItemToObject(root, "rawinfo", raw_item);

    // step 2: Configure
    cJSON *owner_item = cJSON_CreateString("OwnerXYZ");
    cJSON_AddItemToObject(root, "owner", owner_item);
    cJSON *flag_item = cJSON_GetObjectItem(root, "active");
    cJSON *fetched_raw = cJSON_GetObjectItem(root, "rawinfo");
    char *name_str = cJSON_GetStringValue(name_item);

    // step 3: Operate
    cJSON_bool active_true = cJSON_IsTrue(flag_item);
    cJSON_bool raw_is_raw = cJSON_IsRaw(fetched_raw);
    char *owner_str = cJSON_GetStringValue(owner_item);
    size_t buf_len = 128;
    void *buffer = cJSON_malloc(buf_len);
    memset(buffer, 0, buf_len);
    char *snapshot = cJSON_PrintUnformatted(root);
    int combined_flag_sum = (int)active_true + (int)raw_is_raw + (name_str != NULL) + (owner_str != NULL);

    // step 4: Validate & Cleanup
    (void)combined_flag_sum;
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}