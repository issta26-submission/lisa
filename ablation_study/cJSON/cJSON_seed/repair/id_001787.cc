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
//<ID> 1787
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON_AddNumberToObject(settings, "threshold", 42.0);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "name", "device-123");
    cJSON_AddNumberToObject(root, "remove_me", 99.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 3: Operate and Validate
    const char *name_str = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "name"));
    double threshold_val = cJSON_GetNumberValue(cJSON_GetObjectItem(settings, "threshold"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Z';
    buffer[2] = (char)('0' + ((int)threshold_val % 10));
    cJSON_DeleteItemFromObject(root, "remove_me");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}