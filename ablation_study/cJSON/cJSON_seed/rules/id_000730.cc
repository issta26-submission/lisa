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
//<ID> 730
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
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached = NULL;
    char *printed_root = NULL;
    char *printed_detached = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", true_item);
    cJSON_AddItemToObject(root, "neg", false_item);
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());
    cJSON_AddItemToArray(arr, cJSON_CreateFalse());

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "neg");
    printed_root = cJSON_Print(root);
    memcpy(scratch, &arr_size, sizeof(arr_size));
    printed_detached = cJSON_Print(detached);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}