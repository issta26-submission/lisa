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
//<ID> 1783
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

    // step 2: Configure
    cJSON_AddNumberToObject(settings, "max_retries", 5.0);
    cJSON_AddNumberToObject(settings, "timeout", 30.0);
    cJSON_AddStringToObject(settings, "mode", "fast");
    cJSON_AddNumberToObject(root, "version", 1.2);

    // step 3: Operate and Validate
    cJSON *mode_item = cJSON_GetObjectItem(settings, "mode");
    const char *mode_str = cJSON_GetStringValue(mode_item);
    cJSON *timeout_item = cJSON_GetObjectItem(settings, "timeout");
    double timeout_val = cJSON_GetNumberValue(timeout_item);
    cJSON *retries_item = cJSON_GetObjectItem(settings, "max_retries");
    int retries_val = (int)cJSON_GetNumberValue(retries_item);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = mode_str ? mode_str[0] : 'Z';
    buffer[2] = (char)('0' + (retries_val % 10));
    buffer[3] = (char)('0' + ((int)timeout_val % 10));
    (void)buffer;
    (void)mode_str;
    (void)timeout_val;
    (void)retries_val;

    // step 4: Cleanup
    cJSON_DeleteItemFromObject(settings, "max_retries");
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}