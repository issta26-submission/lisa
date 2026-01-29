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
//<ID> 669
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    char *detached_str = (char *)0;
    cJSON_bool added_arr = 0;
    cJSON_bool added_move = 0;
    int arr_size = 0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("one");
    str1 = cJSON_CreateString("two");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    added_arr = cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    detached_str = cJSON_GetStringValue(detached);
    added_move = cJSON_AddItemToObject(root, "moved", detached);
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);
    nonnull_count = (printed != (char *)0) + (detached_str != (char *)0);

    // step 5: Validate
    (void)added_arr;
    (void)added_move;
    (void)arr_size;
    (void)nonnull_count;
    (void)detached_str;
    (void)printed;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}