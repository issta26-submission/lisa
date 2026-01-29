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
//<ID> 654
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *num = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num);
    const char *text = "hello";
    cJSON *sref = cJSON_CreateStringReference(text);
    cJSON_AddItemToArray(arr, sref);

    // step 2: Operate
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    cJSON_AddBoolToObject(root, "first_is_num", first_is_num);
    cJSON *detached = cJSON_DetachItemViaPointer(arr, sref);
    cJSON_bool detached_invalid = cJSON_IsInvalid(detached);
    cJSON_AddBoolToObject(root, "detached_invalid", detached_invalid);

    // step 3: Validate
    int size_after = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);

    // step 4: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}