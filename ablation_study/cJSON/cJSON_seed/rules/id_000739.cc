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
//<ID> 739
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
    char *printed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item_true = NULL;
    cJSON *item_false = NULL;
    cJSON *detached = NULL;
    int size_before = 0;
    int size_after = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item_true = cJSON_CreateTrue();
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, item_true);
    cJSON_AddItemToArray(arr, item_false);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate / Validate
    size_before = cJSON_GetArraySize(arr);
    printed = cJSON_Print(root);
    memcpy(scratch, printed, strlen(printed) + 1);
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "arr");
    size_after = cJSON_GetArraySize(detached);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}