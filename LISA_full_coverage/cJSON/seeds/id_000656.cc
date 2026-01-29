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
//<ID> 656
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "arr");
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, n1);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n2);
    const char *text = "sample";
    cJSON *sref = cJSON_CreateStringReference(text);
    cJSON_AddItemToArray(arr, sref);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");

    // step 2: Configure
    cJSON *item0 = cJSON_GetArrayItem(arr, 0);
    cJSON_bool item0_is_number = cJSON_IsNumber(item0);
    cJSON_AddBoolToObject(meta, "item0_is_number", item0_is_number);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemViaPointer(arr, sref);
    cJSON_AddItemToObject(meta, "detached", detached);
    cJSON_bool detached_is_invalid = cJSON_IsInvalid(detached);
    cJSON_AddBoolToObject(meta, "detached_is_invalid", detached_is_invalid);

    // step 4: Validate & Cleanup
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}