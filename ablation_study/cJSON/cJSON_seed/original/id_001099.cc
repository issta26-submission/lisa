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
//<ID> 1099
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\":[1,2,3],\"maybe_null\":null,\"name\":\"old\",\"num\":3.14}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *new_name = cJSON_CreateString("new_name");
    cJSON_ReplaceItemViaPointer(root, name_item, new_name);

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_bool arr_is_array = cJSON_IsArray(arr);
    int arr_size = cJSON_GetArraySize(arr);
    cJSON *maybe_null = cJSON_GetObjectItem(root, "maybe_null");
    cJSON_bool is_null = cJSON_IsNull(maybe_null);
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    double num_val = cJSON_GetNumberValue(num_item);
    double computed = num_val * (double)arr_size * (double)arr_is_array + (double)is_null;
    cJSON *computed_item = cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}