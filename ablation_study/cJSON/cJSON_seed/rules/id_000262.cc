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
//<ID> 262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *ref_item = NULL;
    cJSON *detached = NULL;
    int arr_size = 0;
    cJSON_bool add_result = 0;
    cJSON_bool cmp_result = 0;
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "values", arr);
    add_result = cJSON_AddItemReferenceToArray(arr, true_item);
    ref_item = cJSON_GetArrayItem(arr, 0);

    // step 3: Operate / Validate
    arr_size = cJSON_GetArraySize(arr);
    cmp_result = cJSON_Compare(true_item, ref_item, 1);
    buffer[0] = (char)('0' + (cmp_result != 0));
    buffer[1] = (char)('0' + (arr_size % 10));
    (void)add_result;
    (void)buffer;

    // step 4: Cleanup
    detached = cJSON_DetachItemViaPointer(arr, ref_item);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // API sequence test completed successfully
    return 66;
}