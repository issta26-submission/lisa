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
//<ID> 664
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
    cJSON_bool added_obj = 0;
    char *detached_val = (char *)0;
    char *printed = (char *)0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str0 = cJSON_CreateString("first");
    str1 = cJSON_CreateString("second");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, str0);
    added1 = cJSON_AddItemToArray(arr, str1);
    added_obj = cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    detached_val = cJSON_GetStringValue(detached);
    printed = cJSON_PrintUnformatted(detached);
    nonnull_count = (detached_val != (char *)0) + (printed != (char *)0) + (added0 != 0) + (added1 != 0) + (added_obj != 0);

    // step 5: Validate
    (void)nonnull_count;
    (void)detached_val;
    (void)added0;
    (void)added1;
    (void)added_obj;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); } /* single-line harmless check for safe free */
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}