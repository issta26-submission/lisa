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
//<ID> 1930
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

    // step 2: Configure
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *dbl_arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(data, "values", dbl_arr);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(data, "name", name);
    cJSON_SetValuestring(name, "updated_name");
    cJSON *null_added = cJSON_AddNullToObject(data, "optional");

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + (cJSON_GetArraySize(dbl_arr) % 10));
    cJSON_bool optional_is_null_before = cJSON_IsNull(null_added);
    buffer[2] = (char)('0' + (optional_is_null_before % 10));
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(data, "optional");
    cJSON *got_optional_after = cJSON_GetObjectItem(data, "optional");
    cJSON_bool optional_is_null_after = cJSON_IsNull(got_optional_after);
    (void)optional_is_null_after;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}