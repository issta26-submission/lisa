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
//<ID> 667
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
    cJSON *s0 = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    char *strval = (char *)0;
    cJSON_bool added_s0 = 0;
    cJSON_bool added_s1 = 0;
    cJSON_bool added_arr = 0;
    int nonnull_count = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("alpha");
    s1 = cJSON_CreateString("beta");

    // step 3: Configure
    added_s0 = cJSON_AddItemToArray(arr, s0);
    added_s1 = cJSON_AddItemToArray(arr, s1);
    added_arr = cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    strval = cJSON_GetStringValue(detached);
    printed = cJSON_PrintUnformatted(detached);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 5: Validate
    nonnull_count = (strval != (char *)0) + (printed != (char *)0) + (added_s0 != 0) + (added_arr != 0);
    (void)nonnull_count;
    (void)added_s1;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}