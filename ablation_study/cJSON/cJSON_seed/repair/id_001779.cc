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
//<ID> 1779
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
    cJSON_AddStringToObject(config, "env", "testing");
    cJSON_AddNumberToObject(config, "version", 7.5);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "author", "alice");
    cJSON_AddNumberToObject(root, "threshold", 42.0);

    // step 2: Configure
    cJSON *notes = cJSON_CreateString("initialization complete");
    cJSON_AddItemToObject(root, "notes", notes);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 3: Operate and Validate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *env_item = cJSON_GetObjectItem(cfg, "env");
    cJSON *ver_item = cJSON_GetObjectItem(cfg, "version");
    cJSON *meta_item = cJSON_GetObjectItem(root, "meta");
    cJSON *author_item = cJSON_GetObjectItem(meta_item, "author");
    cJSON_bool env_is_string = cJSON_IsString(env_item);
    cJSON_bool author_is_string = cJSON_IsString(author_item);
    const char *env_str = cJSON_GetStringValue(env_item);
    const char *author_str = cJSON_GetStringValue(author_item);
    double version_val = cJSON_GetNumberValue(ver_item);
    double threshold_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "threshold"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = env_str ? env_str[0] : 'Z';
    buffer[2] = author_str ? author_str[0] : 'Y';
    buffer[3] = (char)('0' + ((int)version_val % 10));
    buffer[4] = (char)('0' + ((int)threshold_val % 10));
    (void)env_is_string;
    (void)author_is_string;
    (void)env_str;
    (void)author_str;
    (void)version_val;
    (void)threshold_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}