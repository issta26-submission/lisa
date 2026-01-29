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
//<ID> 738
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *item_true = NULL;
    cJSON *item_false = NULL;
    cJSON *meta = NULL;
    cJSON *detached_meta = NULL;
    char *printed_root = NULL;
    char *printed_meta = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    item_true = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, item_true);
    item_false = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, item_false);
    meta = cJSON_CreateObject();
    cJSON_AddItemToObject(meta, "enabled", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "meta", meta);
    detached_meta = cJSON_DetachItemFromObjectCaseSensitive(root, "meta");

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    printed_root = cJSON_Print(root);
    printed_meta = cJSON_Print(detached_meta);

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_meta);
    cJSON_Delete(root);
    cJSON_Delete(detached_meta);

    // API sequence test completed successfully
    return 66;
}