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
//<ID> 735
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
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_true", true_item);
    cJSON_AddItemToObject(root, "flag_false", false_item);
    arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(30.0));
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddStringToObject(root, "description", "array of numbers");

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    memcpy(scratch, &size_before, sizeof(size_before));
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "flag_true");
    printed_root = cJSON_Print(root);
    printed_detached = cJSON_Print(detached);
    size_after = cJSON_GetArraySize(arr);
    memcpy(scratch + sizeof(size_before), &size_after, sizeof(size_after));

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_detached);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    cJSON_free(scratch);

    // API sequence test completed successfully
    return 66;
}