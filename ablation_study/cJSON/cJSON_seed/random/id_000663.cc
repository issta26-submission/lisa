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
//<ID> 663
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
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    char *detached_str = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr = 0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, item0);
    added1 = cJSON_AddItemToArray(arr, item1);
    added_arr = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    detached_str = cJSON_GetStringValue(detached);
    cJSON_AddItemToObject(root, "detached", detached);
    printed = cJSON_PrintUnformatted(root);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)added_arr;
    (void)detached_str;
    (void)printed;
    (void)arr_size;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}