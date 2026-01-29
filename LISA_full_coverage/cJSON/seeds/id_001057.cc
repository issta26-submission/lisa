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
//<ID> 1057
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"arr\":[10,20,30],\"name\":\"example\"}";
    size_t initial_len = sizeof(initial_json) - 1;
    cJSON *root = cJSON_ParseWithLength(initial_json, initial_len);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);

    // step 2: Configure
    cJSON *out = cJSON_CreateObject();
    cJSON_AddStringToObject(out, "source_name", "example");
    cJSON_AddBoolToObject(out, "second_is_invalid", cJSON_IsInvalid(second_item));
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON_AddNumberToObject(out, "second_value_copy", second_value);

    // step 3: Operate
    cJSON *copied_array = cJSON_CreateArray();
    cJSON_AddItemToArray(copied_array, cJSON_Duplicate(cJSON_GetArrayItem(arr, 0), 1));
    cJSON_AddItemToArray(copied_array, cJSON_Duplicate(cJSON_GetArrayItem(arr, 1), 1));
    cJSON_AddItemToArray(copied_array, cJSON_Duplicate(cJSON_GetArrayItem(arr, 2), 1));
    cJSON_AddItemToObject(out, "copied_array", copied_array);
    char *pretty = cJSON_Print(out);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(out, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);
    cJSON_Delete(out);

    // API sequence test completed successfully
    return 66;
}