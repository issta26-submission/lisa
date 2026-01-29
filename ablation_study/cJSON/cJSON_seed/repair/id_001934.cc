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
//<ID> 1934
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *metrics = cJSON_CreateDoubleArray(numbers, 3);
    cJSON_AddItemToObject(root, "metrics", metrics);
    cJSON *optional_null = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "name", name);
    cJSON_SetValuestring(name, "updated_name");

    // step 3: Operate and Validate
    cJSON_bool optional_was_null = cJSON_IsNull(optional_null);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    char *name_value = cJSON_GetStringValue(name);
    buffer[0] = (char)('0' + (optional_was_null % 10));
    buffer[1] = name_value ? name_value[0] : 'X';
    buffer[2] = buffer[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}