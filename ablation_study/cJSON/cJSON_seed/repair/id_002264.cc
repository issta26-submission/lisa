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
//<ID> 2264
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
    cJSON *name = cJSON_CreateString("device42");
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddNumberToObject(config, "count", 3.0);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddStringToObject(meta, "version", "1.0");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *config_clone = cJSON_Duplicate(config, 1);
    cJSON_AddItemToObject(root, "config_clone", config_clone);

    // step 3: Operate and Validate
    cJSON *got_name = cJSON_GetObjectItem(config, "name");
    cJSON *new_name = cJSON_CreateString("device99");
    cJSON_ReplaceItemViaPointer(config, got_name, new_name);
    const char *ver = cJSON_Version();
    char *json = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = ver[0];
    buffer[1] = json[0];
    buffer[2] = (char)('0' + (int)cJSON_GetNumberValue(cJSON_GetObjectItem(config, "count")));
    cJSON_free(buffer);
    cJSON_free(json);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}