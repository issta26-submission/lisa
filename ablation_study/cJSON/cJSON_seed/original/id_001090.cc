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
//<ID> 1090
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1.1,2.2,3.3],\"name\":null,\"num\":3.14}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_check = cJSON_IsArray(arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    cJSON *old_name = cJSON_GetObjectItem(root, "name");
    cJSON_bool name_was_null = cJSON_IsNull(old_name);
    cJSON *new_name = cJSON_CreateString("replaced");
    cJSON_ReplaceItemViaPointer(root, old_name, new_name);
    cJSON *new_first = cJSON_CreateNumber(9.9);
    cJSON_ReplaceItemViaPointer(arr, first_elem, new_first);

    // step 3: Operate
    double first_val = cJSON_GetNumberValue(new_first);
    cJSON *num_field = cJSON_GetObjectItem(root, "num");
    double num_val = cJSON_GetNumberValue(num_field);
    char *pretty = cJSON_Print(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    (void)arr_check;
    (void)name_was_null;
    (void)first_val;
    (void)num_val;
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}