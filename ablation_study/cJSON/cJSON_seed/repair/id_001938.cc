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
//<ID> 1938
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    double nums_arr[3] = {3.14, 2.718, 1.618};
    cJSON *nums = cJSON_CreateDoubleArray(nums_arr, 3);
    cJSON_AddItemToObject(root, "numbers", nums);
    cJSON *label = cJSON_CreateString("initial");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON_SetValuestring(label, "updated_label");
    cJSON *optional_item = cJSON_AddNullToObject(root, "optional");

    // step 3: Operate and Validate
    cJSON_bool optional_is_null = cJSON_IsNull(optional_item);
    cJSON *first_num_item = cJSON_GetArrayItem(nums, 0);
    double first_val = cJSON_GetNumberValue(first_num_item);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 0);
    buffer[0] = (char)('0' + (optional_is_null % 10));
    int intpart = (int)first_val;
    buffer[1] = (char)('0' + (intpart % 10));
    buffer[2] = buffer[1];
    buffer[3] = '\0';

    // step 4: Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}