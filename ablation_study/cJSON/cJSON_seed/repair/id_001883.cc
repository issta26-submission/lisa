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
//<ID> 1883
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
    cJSON_AddTrueToObject(config, "enabled");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(config, "allowed", false_item);
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(config, "values", values);

    // step 2: Configure
    cJSON *enabled_item = cJSON_GetObjectItem(config, "enabled");
    cJSON *allowed_item = cJSON_GetObjectItem(config, "allowed");
    cJSON *values_item = cJSON_GetObjectItem(config, "values");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON_bool allowed_is_bool = cJSON_IsBool(allowed_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (enabled_is_bool % 10));
    buffer[2] = (char)('0' + (allowed_is_bool % 10));
    buffer[3] = (char)('0' + (cJSON_GetArraySize(values_item) % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}