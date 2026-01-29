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
//<ID> 733
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
    cJSON *num_item = NULL;
    cJSON *str_item = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *detached_false = NULL;
    char *printed = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "items");
    num_item = cJSON_CreateNumber(2026.0);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag_true", true_item);
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag_false", false_item);

    // step 3: Operate / Validate
    detached_false = cJSON_DetachItemFromObjectCaseSensitive(root, "flag_false");
    arr_size = cJSON_GetArraySize(arr);
    *((int *)scratch) = arr_size;
    printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(printed);
    cJSON_Delete(detached_false);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}