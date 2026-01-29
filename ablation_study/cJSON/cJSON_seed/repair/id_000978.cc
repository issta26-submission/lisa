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
//<ID> 978
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddNumberToObject(config, "threshold", 0.75);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddNumberToObject(meta, "count", 3.0);
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 3: Operate and Validate
    cJSON *detached = cJSON_DetachItemFromObject(root, "config");
    cJSON_AddItemToObjectCS(root, "config_restored", detached);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *version_item = cJSON_GetObjectItem(meta_ref, "version");
    const char *version_str = cJSON_GetStringValue(version_item);
    cJSON *count_item = cJSON_GetObjectItem(meta_ref, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    cJSON_AddStringToObject(root, "version_snapshot", version_str);
    cJSON_AddNumberToObject(root, "count_snapshot", count_val);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    cJSON_AddBoolToObject(root, "print_ok", printed_ok);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}