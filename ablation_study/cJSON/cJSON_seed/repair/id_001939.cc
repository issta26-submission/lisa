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
//<ID> 1939
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *label = cJSON_CreateString("init_label");
    cJSON_AddItemToObject(meta, "label", label);
    double nums[3] = {1.0, 2.0, 3.0};
    cJSON *arr = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *maybe_null = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON_SetValuestring(label, "renamed_label");
    cJSON_bool was_null = cJSON_IsNull(maybe_null);

    // step 3: Operate and Validate
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON *got_label_item = cJSON_GetObjectItem(meta, "label");
    char *name_value = cJSON_GetStringValue(got_label_item);
    buffer[0] = name_value ? name_value[0] : 'X';
    buffer[1] = (char)('0' + (was_null % 10));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");
    buffer[2] = buffer[0];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}