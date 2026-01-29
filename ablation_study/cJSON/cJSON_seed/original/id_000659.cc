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
//<ID> 659
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *num0 = cJSON_CreateNumber(3.1415);
    cJSON *strref = cJSON_CreateStringReference("hello world");
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num0);
    cJSON_AddItemToArray(items, strref);
    cJSON_AddItemToArray(items, num1);

    // step 2: Configure
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first);
    cJSON_AddBoolToObject(meta, "first_is_number", first_is_number);

    // step 3: Operate
    cJSON *to_detach = cJSON_GetArrayItem(items, 1);
    cJSON *detached = cJSON_DetachItemViaPointer(items, to_detach);
    cJSON_bool detached_is_invalid = cJSON_IsInvalid(detached);
    cJSON_AddBoolToObject(meta, "detached_is_invalid", detached_is_invalid);
    cJSON_AddItemToObject(root, "detached", detached);

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