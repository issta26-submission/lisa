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
//<ID> 660
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
    char *s0_val = (char *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_obj = 0;
    cJSON_bool added_detached = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s0 = cJSON_CreateString("one");
    s1 = cJSON_CreateString("two");

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, s0);
    added1 = cJSON_AddItemToArray(arr, s1);
    added_obj = cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 1);
    added_detached = cJSON_AddItemToObject(root, "second", detached);
    s0_val = cJSON_GetStringValue(s0);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)added_obj;
    (void)added_detached;
    (void)s0_val;
    (void)printed;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}