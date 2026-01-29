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
//<ID> 803
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
    cJSON *true_item_root = cJSON_CreateTrue();
    cJSON *true_item_config = cJSON_CreateTrue();
    cJSON *num_item = cJSON_CreateNumber(42.0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "flag", true_item_root);
    cJSON_AddItemToObject(root, "value", num_item);
    cJSON_AddItemToObject(config, "enabled", true_item_config);
    cJSON_AddItemToObject(root, "config", config);
    cJSON_AddRawToObject(root, "payload", "{\"embedded\":1}");

    // step 3: Operate
    cJSON *found_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    cJSON_bool flag_true = cJSON_IsTrue(found_flag);
    cJSON_AddNumberToObject(root, "flag_as_number", (double)flag_true);
    cJSON *found_payload = cJSON_GetObjectItemCaseSensitive(root, "payload");
    cJSON_bool payload_is_raw = cJSON_IsRaw(found_payload);
    cJSON_AddNumberToObject(root, "payload_is_raw", (double)payload_is_raw);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}