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
//<ID> 91
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
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *item3 = (cJSON *)0;
    cJSON *flag_false = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *detached_str = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item1 = cJSON_CreateString("alpha");
    item2 = cJSON_CreateString("beta");
    item3 = cJSON_CreateString("gamma");
    flag_false = cJSON_CreateFalse();

    // step 3: Configure - populate array and attach structures to root
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToArray(arr, item3);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "flag", flag_false);

    // step 4: Operate - detach second element from the array and read its string
    detached = cJSON_DetachItemFromArray(arr, 1);
    detached_str = cJSON_GetStringValue(detached);

    // step 5: Validate - record the detached value under root and remove the flag object
    cJSON_AddItemToObject(root, "detached_value", cJSON_CreateString(detached_str ? detached_str : ""));
    cJSON_DeleteItemFromObject(root, "flag");

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}