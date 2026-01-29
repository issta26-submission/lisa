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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "object", obj);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(obj, "value", num);
    cJSON_AddRawToObject(obj, "raw", "{\"inner\":true}");

    // step 2: Configure
    cJSON *val = cJSON_GetObjectItem(obj, "value");
    double base = cJSON_GetNumberValue(val);
    cJSON *updated = cJSON_CreateNumber(base + 8.0);
    cJSON_AddItemToObject(obj, "value_updated", updated);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemViaPointer(obj, val);
    cJSON_AddItemToObject(root, "detached_value", detached);
    cJSON *rawptr = cJSON_GetObjectItem(obj, "raw");
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}