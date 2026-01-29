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
//<ID> 1479
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *mirror = cJSON_CreateObject();
    cJSON *nested1 = cJSON_CreateObject();
    cJSON *nested2 = cJSON_CreateObject();
    cJSON_AddStringToObject(nested1, "k", "v");
    cJSON_AddNumberToObject(nested1, "num", 7.5);
    cJSON_AddItemToObject(root, "nested", nested1);
    cJSON_AddStringToObject(root, "temp", "remove_this");
    cJSON_AddStringToObject(nested2, "k", "v");
    cJSON_AddNumberToObject(nested2, "num", 7.5);
    cJSON_AddItemToObject(mirror, "nested", nested2);
    cJSON_AddStringToObject(mirror, "temp", "remove_this");

    // step 2: Configure
    char *printed_root_before = cJSON_PrintUnformatted(root);
    cJSON_bool root_is_object = cJSON_IsObject(root);
    cJSON_bool equal_before = cJSON_Compare(root, mirror, 1);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root, "temp");
    char *printed_detached = cJSON_PrintUnformatted(detached);
    cJSON_DeleteItemFromObjectCaseSensitive(mirror, "temp");
    cJSON_bool equal_after = cJSON_Compare(root, mirror, 1);

    // step 4: Cleanup
    cJSON_free(printed_root_before);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_Delete(mirror);

    // API sequence test completed successfully
    (void)root_is_object;
    (void)equal_before;
    (void)equal_after;
    return 66;
}