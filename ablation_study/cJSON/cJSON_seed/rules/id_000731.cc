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
//<ID> 731
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(128);
    memset(scratch, 0, 128);
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached = NULL;
    char *json_root = NULL;
    char *json_detached = NULL;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "flags", arr);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    json_root = cJSON_Print(root);
    memcpy(scratch, &size_before, sizeof(size_before));
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "flags");
    size_after = cJSON_GetArraySize(detached);
    json_detached = cJSON_Print(detached);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(json_root);
    cJSON_free(json_detached);
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}