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
//<ID> 1780
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
    cJSON_AddStringToObject(config, "env", "production");
    cJSON_AddNumberToObject(config, "version", 1.2);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "devops");

    // step 2: Configure
    cJSON_AddNumberToObject(root, "threshold", 100.0);
    cJSON *notes = cJSON_CreateString("bootstrap complete");
    cJSON_AddItemToObject(root, "notes", notes);

    // step 3: Operate and Validate
    cJSON *cfg_item = cJSON_GetObjectItem(root, "config");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    const char *env_str = cJSON_GetStringValue(cJSON_GetObjectItem(cfg_item, "env"));
    const char *author_str = cJSON_GetStringValue(cJSON_GetObjectItem(meta_item, "author"));
    double version_val = cJSON_GetNumberValue(cJSON_GetObjectItem(cfg_item, "version"));
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = env_str ? env_str[0] : 'Z';
    buffer[2] = author_str ? author_str[0] : 'Y';
    buffer[3] = (char)('0' + ((int)version_val % 10));
    cJSON_DeleteItemFromObject(root, "notes");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}