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
//<ID> 1602
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
    cJSON *true_item = NULL;
    cJSON *rawitem = NULL;
    cJSON *arr_ref = NULL;
    cJSON *holder = NULL;
    cJSON *detached_raw = NULL;
    cJSON *copy_str = NULL;
    char *out = NULL;
    int arr_size = 0;
    const char *detached_value = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    rawitem = cJSON_CreateRaw("{\"embedded\":42}");
    cJSON_AddItemToObject(root, "raw", rawitem);
    arr_ref = cJSON_CreateArrayReference(arr);
    holder = cJSON_CreateObject();
    cJSON_AddItemToObject(holder, "list_ref", arr_ref);

    // step 3: Operate / Validate
    detached_raw = cJSON_DetachItemFromObject(root, "raw");
    detached_value = cJSON_GetStringValue(detached_raw);
    copy_str = cJSON_CreateString(detached_value);
    cJSON_AddItemToObject(root, "detached_copy", copy_str);
    arr_size = cJSON_GetArraySize(arr);
    out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(detached_raw);
    cJSON_Delete(holder);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}