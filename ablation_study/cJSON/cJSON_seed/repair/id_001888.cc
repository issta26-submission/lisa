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
//<ID> 1888
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
    cJSON *enabled = cJSON_AddTrueToObject(data, "enabled");
    double numbers[3] = {1.5, 2.5, 3.5};
    cJSON *values = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(data, "values", values);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(data, "flag", flag);

    // step 2: Configure
    cJSON *data_item = cJSON_GetObjectItem(root, "data");
    cJSON *flag_item = cJSON_GetObjectItem(data_item, "flag");
    cJSON_bool flag_is_bool = cJSON_IsBool(flag_item);
    cJSON *enabled_item = cJSON_GetObjectItem(data_item, "enabled");
    cJSON_bool enabled_is_bool = cJSON_IsBool(enabled_item);
    cJSON *values_item = cJSON_GetObjectItem(data_item, "values");
    int arr_size = cJSON_GetArraySize(values_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (flag_is_bool % 10));
    buffer[2] = (char)('0' + (enabled_is_bool % 10));
    buffer[3] = (char)('0' + (arr_size % 10));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}