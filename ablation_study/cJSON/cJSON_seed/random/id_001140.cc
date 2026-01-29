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
//<ID> 1140
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
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *detachable = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *fetched_numbers = (cJSON *)0;
    cJSON *arr_item1 = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *fetched_label = (cJSON *)0;
    char *printed = (char *)0;
    double v_arr1 = 0.0;
    double v_detached = 0.0;
    int validation_score = 0;

    // step 2: Setup - create root, array, numeric elements, a detachable element and a string label
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", arr);
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.72);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    label = cJSON_CreateString("example-label");
    cJSON_AddItemToObject(root, "label", label);
    detachable = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "detachable", detachable);

    // step 3: Operate - retrieve object and array elements, detach a child by pointer, read numbers
    fetched_numbers = cJSON_GetObjectItem(root, "numbers");
    arr_item1 = cJSON_GetArrayItem(fetched_numbers, 1);
    v_arr1 = cJSON_GetNumberValue(arr_item1);
    fetched_label = cJSON_GetObjectItem(root, "label");
    detached = cJSON_DetachItemViaPointer(root, detachable);
    v_detached = cJSON_GetNumberValue(detached);

    // step 4: Validate - serialize root for a round-trip and compute a simple validation score
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(v_arr1 == 2.72) + (int)(v_detached == 42.0) + (int)(fetched_label != (cJSON *)0);
    (void)validation_score;

    // step 5: Cleanup - delete root and the detached item
    cJSON_Delete(root);
    cJSON_Delete(detached);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}