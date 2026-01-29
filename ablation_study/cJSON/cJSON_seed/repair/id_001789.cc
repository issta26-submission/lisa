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
//<ID> 1789
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
    cJSON_AddNumberToObject(config, "threshold", 7.5);
    cJSON_AddNumberToObject(root, "version", 3.0);
    cJSON *desc = cJSON_CreateString("sample-description");
    cJSON_AddItemToObject(root, "desc", desc);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddNumberToObject(meta, "count", 42.0);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);

    // step 3: Operate and Validate
    const char *desc_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "desc"));
    double version_val = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "version"));
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = desc_str ? desc_str[0] : 'Z';
    buffer[2] = (char)('0' + ((int)version_val % 10));
    buffer[3] = cJSON_HasObjectItem(config, "threshold") ? '1' : '0';
    cJSON_DeleteItemFromObject(config, "threshold");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}