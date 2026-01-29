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
//<ID> 736
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *scratch = (char *)cJSON_malloc(256);
    memset(scratch, 0, 256);
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *t = NULL;
    cJSON *f = NULL;
    cJSON *detached = NULL;
    char *printed_root = NULL;
    char *printed_detached = NULL;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    t = cJSON_CreateTrue();
    f = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, f);
    cJSON_AddItemToObject(root, "flags", arr);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    *((int *)scratch) = size_before;
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "flags");
    size_after = cJSON_GetArraySize(detached);
    *((int *)(scratch + sizeof(int))) = size_after;
    printed_root = cJSON_Print(root);
    printed_detached = cJSON_Print(detached);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}