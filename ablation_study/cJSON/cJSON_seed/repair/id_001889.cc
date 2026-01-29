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
//<ID> 1889
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "data", data);
    cJSON *true_item = cJSON_AddTrueToObject(data, "enabled");
    cJSON *false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(data, "disabled", false_item);
    double numbers[3] = {3.14, 2.71, -1.0};
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(data, "values", dbl_array);

    // step 2: Configure
    cJSON *got_enabled = cJSON_GetObjectItem(data, "enabled");
    cJSON *got_disabled = cJSON_GetObjectItem(data, "disabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(got_enabled);
    cJSON_bool disabled_is_bool = cJSON_IsBool(got_disabled);
    int values_count = cJSON_GetArraySize(cJSON_GetObjectItem(data, "values"));
    char *printed = cJSON_PrintUnformatted(root);

    // step 3: Operate and Validate
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (enabled_is_bool % 10));
    buffer[2] = (char)('0' + (disabled_is_bool % 10));
    buffer[3] = (char)('0' + (values_count % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}