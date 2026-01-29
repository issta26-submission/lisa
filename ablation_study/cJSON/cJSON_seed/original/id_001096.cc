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
//<ID> 1096
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"name\":\"old\",\"num\":5}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("new_name");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);
    cJSON *maybe_null = cJSON_AddNullToObject(root, "maybe_null");
    cJSON *arr = cJSON_GetObjectItem(root, "arr");

    // step 3: Operate
    cJSON_bool null_check = cJSON_IsNull(maybe_null);
    cJSON_bool array_check = cJSON_IsArray(arr);
    cJSON *first_elem = cJSON_GetArrayItem(arr, 0);
    double first_value = cJSON_GetNumberValue(first_elem);
    char *compact = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 0);

    // step 4: Validate & Cleanup
    cJSON_free(compact);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}