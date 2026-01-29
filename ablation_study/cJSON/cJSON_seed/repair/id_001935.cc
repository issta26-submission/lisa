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
//<ID> 1935
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
    cJSON *name_item = cJSON_CreateString("initial");
    cJSON_AddItemToObject(config, "name", name_item);
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(config, "values", dbl_arr);
    cJSON *optional = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    char *updated_ptr = cJSON_SetValuestring(name_item, "updated_name");
    (void)updated_ptr;
    cJSON *temp = cJSON_CreateString("tempval");
    cJSON_AddItemToObject(root, "temp", temp);

    // step 3: Operate and Validate
    cJSON_bool optional_is_null = cJSON_IsNull(optional);
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (optional_is_null % 10));
    const char *name_str = cJSON_GetStringValue(name_item);
    buffer[2] = name_str ? name_str[0] : 'X';
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}