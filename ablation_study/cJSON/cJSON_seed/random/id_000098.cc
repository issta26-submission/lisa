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
//<ID> 98
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
    cJSON *item2 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *moved_arr = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    char *detached_str = (char *)0;

    // step 2: Setup - create root object and an array with three string items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateString("alpha");
    item1 = cJSON_CreateString("beta");
    item2 = cJSON_CreateString("gamma");

    // step 3: Configure - populate the array and attach it to the root
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToArray(arr, item2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate - detach the second item from the array, read its string, and move it to a new array
    detached = cJSON_DetachItemFromArray(arr, 1);
    detached_str = cJSON_GetStringValue(detached);
    moved_arr = cJSON_CreateArray();
    cJSON_AddItemToArray(moved_arr, detached);
    cJSON_AddItemToObject(root, "moved", moved_arr);

    // step 5: Validate - record the detached string and a false flag in the root, then remove the original items object
    cJSON_AddItemToObject(root, "detached_value", cJSON_CreateString(detached_str));
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_DeleteItemFromObject(root, "items");

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}