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
//<ID> 1885
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flags = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "flags", flags);
    cJSON_AddTrueToObject(flags, "active");
    cJSON_AddItemToObject(flags, "disabled", cJSON_CreateFalse());
    double nums[3] = {1.5, 2.5, 3.5};
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *flags_item = cJSON_GetObjectItem(root, "flags");
    cJSON *active_item = cJSON_GetObjectItem(flags_item, "active");
    cJSON *disabled_item = cJSON_GetObjectItem(flags_item, "disabled");
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    cJSON_bool active_is_bool = cJSON_IsBool(active_item);
    cJSON_bool disabled_is_bool = cJSON_IsBool(disabled_item);
    int values_count = cJSON_GetArraySize(values_item);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (active_is_bool % 10));
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