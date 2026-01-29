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
//<ID> 1937
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
    cJSON *label = cJSON_CreateString("initial_label");
    cJSON_AddItemToObject(data, "label", label);
    double nums1[3] = {1.23, 4.56, 7.89};
    cJSON *values = cJSON_CreateDoubleArray(nums1, 3);
    cJSON_AddItemToObject(data, "values", values);
    cJSON *opt_null = cJSON_AddNullToObject(root, "optional");

    // step 2: Configure
    cJSON_SetValuestring(label, "updated_label");
    double nums2[2] = {9.90, 8.80};
    cJSON *new_values = cJSON_CreateDoubleArray(nums2, 2);
    cJSON_ReplaceItemInObject(data, "values", new_values);

    // step 3: Operate and Validate
    cJSON *got_values = cJSON_GetObjectItem(data, "values");
    cJSON *first_item = cJSON_GetArrayItem(got_values, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON *got_optional = cJSON_GetObjectItem(root, "optional");
    cJSON_bool optional_is_null = cJSON_IsNull(got_optional);
    const int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    char *printed = cJSON_PrintUnformatted(root);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = (char)('0' + ((int)first_val % 10 + 10) % 10);
    buffer[2] = (char)('0' + (optional_is_null % 10));
    buffer[3] = '\0';
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}