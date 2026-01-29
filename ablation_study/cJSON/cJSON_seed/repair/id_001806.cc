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
//<ID> 1806
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first = cJSON_CreateObject();
    cJSON_AddItemToObject(first, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(first, "status", cJSON_CreateString("ok"));
    cJSON_AddNumberToObject(first, "value", 3.14);
    cJSON_AddItemToArray(items, first);

    // step 2: Configure
    cJSON *second = cJSON_CreateObject();
    cJSON_AddItemToObject(second, "name", cJSON_CreateString("beta"));
    cJSON_AddItemToObject(second, "status", cJSON_CreateString("degraded"));
    cJSON_AddNumberToObject(second, "value", 7.0);
    cJSON_AddItemToArray(items, second);
    cJSON_AddItemToObject(root, "description", cJSON_CreateString("two-items-list"));
    cJSON *config = cJSON_CreateObject();
    cJSON_AddNumberToObject(config, "threshold", 42.0);
    cJSON_AddItemToObject(root, "config", config);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(256);
    memset(buffer, 0, 256);
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *name_item = cJSON_GetObjectItem(first_item, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *desc_item = cJSON_GetObjectItem(root, "description");
    const char *desc_str = cJSON_GetStringValue(desc_item);
    cJSON *config_item = cJSON_GetObjectItem(root, "config");
    cJSON *threshold_item = cJSON_GetObjectItem(config_item, "threshold");
    double threshold = cJSON_GetNumberValue(threshold_item);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_str ? name_str[0] : 'Y';
    buffer[2] = desc_str ? desc_str[0] : 'Z';
    buffer[3] = (char)('0' + ((int)threshold % 10));
    (void)buffer; (void)name_str; (void)desc_str; (void)threshold;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}