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
//<ID> 107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("one");
    cJSON *s2 = cJSON_CreateString("two");
    cJSON *replacement = cJSON_CreateNumber(42.0);
    cJSON *detached = (cJSON *)0;
    cJSON *got_child = (cJSON *)0;
    int arr_size = 0;

    // step 2: Setup - populate structures and attach them to root
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "letters", arr);
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddStringToObject(child, "inner", "data");

    // step 3: Operate - query size, replace an object, detach the array, and retrieve the replaced item
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(root, "letters"));
    cJSON_ReplaceItemInObject(root, "child", replacement);
    detached = cJSON_DetachItemFromObject(root, "letters");
    got_child = cJSON_GetObjectItem(root, "child");

    // step 4: Validate - record the observed array size into the root object
    cJSON_AddNumberToObject(root, "letters_count", (double)arr_size);

    // step 5: Cleanup - delete detached structures and the root
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}