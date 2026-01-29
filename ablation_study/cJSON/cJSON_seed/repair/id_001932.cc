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
//<ID> 1932
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
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(meta, "name", name);
    double nums[3] = {1.1, 2.2, 3.3};
    cJSON *values = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *optional = cJSON_AddNullToObject(root, "optional");
    cJSON *note = cJSON_CreateString("temp_note");
    cJSON_AddItemToObject(root, "note", note);
    cJSON_SetValuestring(note, "final_note");

    // step 3: Operate and Validate
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, buflen);
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *first_value_item = cJSON_GetArrayItem(values, 0);
    double first_val = cJSON_GetNumberValue(first_value_item);
    buffer[0] = (char)('0' + ((int)first_val % 10));
    cJSON_bool optional_is_null = cJSON_IsNull(optional);
    buffer[1] = (char)('0' + (optional_is_null % 10));
    cJSON_DeleteItemFromObjectCaseSensitive(root, "optional");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}