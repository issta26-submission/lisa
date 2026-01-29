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
//<ID> 1770
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
    cJSON_AddStringToObject(config, "name", "example-service");
    cJSON_AddNumberToObject(config, "version", 3.14);
    cJSON *features = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "features", features);
    cJSON *f1 = cJSON_CreateString("logging");
    cJSON_AddItemToArray(features, f1);
    cJSON *f2 = cJSON_CreateString("metrics");
    cJSON_AddItemToArray(features, f2);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "owner", "dev-team");
    cJSON_AddNumberToObject(meta, "instances", 5);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 3: Operate and Validate
    cJSON *cfg = cJSON_GetObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItem(cfg, "name");
    cJSON *version_item = cJSON_GetObjectItem(cfg, "version");
    cJSON *meta_obj = cJSON_GetObjectItem(root, "meta");
    cJSON *owner_item = cJSON_GetObjectItem(meta_obj, "owner");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    const char *name_str = cJSON_GetStringValue(name_item);
    double version_val = cJSON_GetNumberValue(version_item);
    const char *owner_str = cJSON_GetStringValue(owner_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)name_is_string);
    buffer[2] = (char)('0' + ((int)version_val % 10));
    buffer[3] = owner_str ? owner_str[0] : 'Z';
    (void)name_str;
    (void)owner_str;
    (void)version_val;
    (void)name_is_string;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}