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
//<ID> 1931
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
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(data, "values", values);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(data, "name", name);
    cJSON *opt = cJSON_AddNullToObject(data, "optional");
    cJSON_SetValuestring(name, "updated");

    // step 3: Operate and Validate
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON *got_optional = cJSON_GetObjectItem(data, "optional");
    cJSON_bool is_null = cJSON_IsNull(got_optional);
    int size_values = cJSON_GetArraySize(cJSON_GetObjectItem(data, "values"));
    buffer[0] = (char)('0' + (is_null % 10));
    buffer[1] = (char)('0' + (size_values % 10));
    buffer[2] = buffer[1];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(data, "optional");
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}