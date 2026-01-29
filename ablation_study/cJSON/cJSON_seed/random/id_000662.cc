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
//<ID> 662
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
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr = 0;
    char *moved_value = (char *)0;
    char *json_unformatted = (char *)0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("first");
    str1 = cJSON_CreateString("second");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, str0);
    added1 = cJSON_AddItemToArray(arr, str1);
    added_arr = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    cJSON_AddItemToObject(root, "moved", detached);
    moved_value = cJSON_GetStringValue(detached);
    json_unformatted = cJSON_PrintUnformatted(root);

    // step 5: Validate
    nonnull_count = (moved_value != (char *)0) + (json_unformatted != (char *)0);
    (void)added0;
    (void)added1;
    (void)added_arr;
    (void)moved_value;
    (void)json_unformatted;
    (void)nonnull_count;

    // step 6: Cleanup
    if (json_unformatted) { cJSON_free(json_unformatted); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}