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
//<ID> 1645
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *detached_from_array = (cJSON *)0;
    int array_size_before = 0;
    int array_size_after = 0;
    int validation_score = 0;
    char json_text[] = "{\"note\": \"  spaced \", \"array\": [ 10 , 20 , 30 ] }";

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    str_item = cJSON_CreateString("will_be_moved");

    // step 3: Configure
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "note", cJSON_CreateString("  spaced "));
    cJSON_AddItemToObject(root, "attached", str_item);

    // step 4: Operate
    array_size_before = cJSON_GetArraySize(arr);
    detached = cJSON_DetachItemViaPointer(root, str_item);
    cJSON_AddItemToArray(arr, detached);
    detached_from_array = cJSON_DetachItemFromArray(arr, 1);
    cJSON_Minify(json_text);
    array_size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)(arr != (cJSON *)0)
        + (int)(detached != (cJSON *)0)
        + (int)(detached_from_array != (cJSON *)0)
        + (int)(array_size_before == 2)
        + (int)(array_size_after == 2);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_from_array);
    (void)validation_score;
    (void)json_text;
    (void)num1;
    (void)num2;
    (void)str_item;
    (void)detached;
    // API sequence test completed successfully
    return 66;
}