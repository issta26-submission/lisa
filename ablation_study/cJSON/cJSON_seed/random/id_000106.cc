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
//<ID> 106
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
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *num = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *newitem = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON_bool replace_result = 0;
    int array_size = 0;
    const char *retrieved_value = (const char *)0;

    // step 2: Setup - initialize root object, array and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s1 = cJSON_CreateString("one");
    s2 = cJSON_CreateString("two");
    num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON_AddItemToObject(root, "myArray", arr);
    cJSON_AddItemToObject(root, "toReplace", num);

    // step 3: Operate - replace an object member and detach the array
    retrieved = cJSON_GetObjectItem(root, "toReplace");
    newitem = cJSON_CreateString("replaced");
    replace_result = cJSON_ReplaceItemInObject(root, "toReplace", newitem);
    detached = cJSON_DetachItemFromObject(root, "myArray");
    array_size = cJSON_GetArraySize(detached);
    cJSON_AddNumberToObject(root, "detached_size", (double)array_size);

    // step 4: Validate - record the replaced value into the root for inspection
    retrieved = cJSON_GetObjectItem(root, "toReplace");
    retrieved_value = cJSON_GetStringValue(retrieved);
    cJSON_AddStringToObject(root, "toReplace_value", retrieved_value);

    // step 5: Cleanup - free detached structure and the remaining root
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}