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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"arr\": [ true, false, true ], \"obj\": { \"key\": \"val\" } }";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, (size_t)(sizeof(json) - 1), &parse_end, 1);

    // step 2: Configure
    cJSON_AddStringToObject(root, "ver", cJSON_Version());
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON *new_true = cJSON_CreateTrue();
    cJSON_bool replaced = cJSON_ReplaceItemInObjectCaseSensitive(root, "obj", new_true);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_true = cJSON_IsTrue(first);
    cJSON_DeleteItemFromArray(arr, 1);
    int size_after = cJSON_GetArraySize(arr);
    cJSON_AddBoolToObject(root, "first_true", first_is_true);
    cJSON_AddBoolToObject(root, "obj_replaced", replaced);
    cJSON_AddNumberToObject(root, "arr_size", (double)size_after);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}