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
//<ID> 1880
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
    cJSON_AddTrueToObject(settings, "enabled");
    cJSON_AddItemToObject(settings, "disabled", cJSON_CreateFalse());
    double numbers[3];
    numbers[0] = 1.1;
    numbers[1] = 2.2;
    numbers[2] = 3.3;
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *enabled_item = cJSON_GetObjectItem(settings, "enabled");
    cJSON *disabled_item = cJSON_GetObjectItem(settings, "disabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON_bool disabled_is_bool = cJSON_IsBool(disabled_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (int)(enabled_is_bool % 10));
    buffer[2] = (char)('0' + (int)(disabled_is_bool % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}