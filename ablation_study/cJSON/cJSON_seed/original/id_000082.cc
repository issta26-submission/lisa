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
//<ID> 82
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
    cJSON_AddStringToObject(meta, "name", "example");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON *detachable = cJSON_CreateObject();
    cJSON_AddStringToObject(detachable, "note", "will be detached");
    cJSON_AddItemToObject(root, "detachable", detachable);

    // step 3: Operate and Validate
    char *printed_root = cJSON_Print(root);
    cJSON *detached = cJSON_DetachItemViaPointer(root, detachable);
    cJSON_bool detached_is_array = cJSON_IsArray(detached);
    cJSON_bool values_is_array = cJSON_IsArray(cJSON_GetObjectItem(root, "values"));
    char *printed_detached = cJSON_Print(detached);
    int summary = (int)detached_is_array + (int)values_is_array + (int)printed_root[0] + (int)printed_detached[0];
    (void)summary;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}