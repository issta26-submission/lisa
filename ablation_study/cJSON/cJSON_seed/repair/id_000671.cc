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
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *config = cJSON_AddObjectToObject(root, "config");

    // step 2: Configure
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddNumberToObject(meta, "version", 1.0);
    cJSON_AddStringToObject(config, "mode", "test");
    cJSON_AddNumberToObject(config, "timeout", 30.0);

    // step 3: Operate and Validate
    cJSON *meta_copy = cJSON_Duplicate(meta, 1);
    cJSON_AddItemToObject(root, "meta_copy", meta_copy);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(meta, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    cJSON_AddBoolToObject(config, "name_is_string", name_is_string);
    char *snapshot = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON_PrintPreallocated(root, buffer, 256, 1);
    cJSON_AddStringToObject(root, "snapshot_buffer", buffer);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}