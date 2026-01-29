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
//<ID> 672
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON_AddStringToObject(config, "name", "fuzzer");
    cJSON_AddNumberToObject(config, "version", 1.23);
    cJSON_AddBoolToObject(config, "enabled", 1);
    cJSON *meta = cJSON_AddObjectToObject(config, "meta");
    cJSON_AddStringToObject(meta, "author", "tester");

    // step 2: Configure
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddStringToObject(extra, "note", "additional info");
    cJSON_AddItemToObject(config, "extra", extra);
    cJSON_AddNumberToObject(extra, "count", 7.0);

    // step 3: Operate and Validate
    cJSON *config_copy = cJSON_Duplicate(config, 1);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(config_copy, "name");
    cJSON_bool name_is_str = cJSON_IsString(name_item);
    cJSON_AddBoolToObject(root, "name_is_string", name_is_str);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)printed;
    cJSON_AddStringToObject(root, "snapshot", buffer);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(config_copy);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}