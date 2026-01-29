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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *array = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *item3 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const char *detached_val = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    array = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");
    item3 = cJSON_CreateString("gamma");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach to root, add a false flag
    cJSON_AddItemToArray(array, item1);
    cJSON_AddItemToArray(array, item2);
    cJSON_AddItemToArray(array, item3);
    cJSON_AddItemToObject(root, "items", array);
    cJSON_AddItemToObject(root, "ok_flag", flag_false);

    // step 4: Operate - detach the middle element from the array and move it into the root
    detached = cJSON_DetachItemFromArray(array, 1);
    detached_val = cJSON_GetStringValue(detached);
    cJSON_AddItemToObject(root, "moved", detached);

    // step 5: Validate - record the detached string as a separate copy, then remove the original array from the root
    cJSON_AddStringToObject(root, "moved_copy", detached_val);
    cJSON_DeleteItemFromObject(root, "items");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}