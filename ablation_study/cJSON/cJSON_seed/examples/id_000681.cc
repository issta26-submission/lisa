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
//<ID> 681
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *str_item = cJSON_CreateString("hello");

    // step 2: Configure
    cJSON_AddItemToArray(array, num0);
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, str_item);
    cJSON_AddItemToObject(root, "items", array);
    cJSON *array_dup = cJSON_Duplicate(array, 1);

    // step 3: Operate & Validate
    int original_size = cJSON_GetArraySize(array);
    cJSON *second_in_original = cJSON_GetArrayItem(array, 1);
    cJSON *first_in_dup = cJSON_GetArrayItem(array_dup, 0);
    cJSON_bool arrays_equal = cJSON_Compare(array, array_dup, 1);
    cJSON_bool items_equal = cJSON_Compare(cJSON_GetArrayItem(array, 0), first_in_dup, 1);
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t snap_len = 0;
    if (snapshot) {
        snap_len = strlen(snapshot);
    }
    char *buffer = (char *)cJSON_malloc(snap_len + 1);
    memset(buffer, 0, snap_len + 1);
    if (snapshot) {
        memcpy(buffer, snapshot, snap_len + 1);
    }
    (void)original_size;
    (void)second_in_original;
    (void)arrays_equal;
    (void)items_equal;
    (void)snap_len;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(array_dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}