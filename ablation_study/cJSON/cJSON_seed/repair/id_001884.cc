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
//<ID> 1884
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);
    cJSON *active = cJSON_AddTrueToObject(root, "active");
    cJSON *disabled = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "disabled", disabled);

    // step 2: Configure
    cJSON *active_item = cJSON_GetObjectItem(root, "active");
    cJSON *disabled_item = cJSON_GetObjectItem(root, "disabled");
    cJSON_bool active_is_bool = cJSON_IsBool(active_item);
    cJSON_bool disabled_is_bool = cJSON_IsBool(disabled_item);

    // step 3: Operate & Validate
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (active_is_bool % 10));
    buffer[2] = (char)('0' + (disabled_is_bool % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}