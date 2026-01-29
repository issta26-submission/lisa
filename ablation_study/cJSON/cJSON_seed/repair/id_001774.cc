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
//<ID> 1774
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
    cJSON_AddStringToObject(config, "name", "test-suite");
    cJSON_AddNumberToObject(config, "version", 1.2);
    cJSON *features = cJSON_CreateArray();
    cJSON_AddItemToObject(config, "features", features);
    cJSON_AddItemToArray(features, cJSON_CreateString("featureA"));
    cJSON_AddItemToArray(features, cJSON_CreateString("featureB"));
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "source", "sensor-X");
    cJSON_AddTrueToObject(meta, "enabled");

    // step 2: Configure
    cJSON *extra = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON_AddStringToObject(extra, "note", "auto-generated");
    cJSON_AddNumberToObject(extra, "priority", 5.0);

    // step 3: Operate and Validate
    cJSON *config_ref = cJSON_GetObjectItem(root, "config");
    cJSON *name_item = cJSON_GetObjectItem(config_ref, "name");
    cJSON_bool name_is_string = cJSON_IsString(name_item);
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *version_item = cJSON_GetObjectItem(config_ref, "version");
    double version_value = cJSON_GetNumberValue(version_item);
    cJSON *meta_ref = cJSON_GetObjectItem(root, "meta");
    cJSON *source_item = cJSON_GetObjectItem(meta_ref, "source");
    char *source_value = cJSON_GetStringValue(source_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(name_is_string ? 1 : 0));
    buffer[2] = name_value ? name_value[0] : 'Z';
    buffer[3] = (char)('0' + (int)((int)version_value % 10));
    buffer[4] = source_value ? source_value[0] : 'Y';
    (void)buffer;
    (void)name_value;
    (void)source_value;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}