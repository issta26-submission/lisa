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
//<ID> 931
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "child", child);
    cJSON *num = cJSON_CreateNumber(7.0);
    cJSON_AddItemToObject(child, "num", num);
    cJSON *raw_old = cJSON_CreateRaw("{\"version\":\"1.0\"}");
    cJSON_AddItemToObject(root, "raw", raw_old);

    // step 2: Configure
    cJSON *raw_new = cJSON_CreateRaw("{\"version\":\"2.0\",\"updated\":true}");
    cJSON *name = cJSON_CreateString("device_alpha");
    cJSON_AddItemToObject(root, "name", name);

    // step 3: Operate and Validate
    cJSON *found_raw = cJSON_GetObjectItemCaseSensitive(root, "raw");
    cJSON_ReplaceItemViaPointer(root, found_raw, raw_new);
    cJSON *found_num = cJSON_GetObjectItemCaseSensitive(child, "num");
    double num_value = cJSON_GetNumberValue(found_num);
    (void)num_value;
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}