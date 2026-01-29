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
//<ID> 1146
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
    cJSON *label = (cJSON *)0;
    cJSON *fetched_item = (cJSON *)0;
    cJSON *fetched_count = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double v_item = 0.0;
    double v_count = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and attach an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);

    // step 3: Configure - create numeric items and a string, add them to structures
    num0 = cJSON_CreateNumber(2.0);
    num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    label = cJSON_CreateString("example");
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddNumberToObject(root, "count", 7.0);

    // step 4: Operate - retrieve an array element and an object member, read numeric values, detach an item
    fetched_item = cJSON_GetArrayItem(arr, 1);
    v_item = cJSON_GetNumberValue(fetched_item);
    fetched_count = cJSON_GetObjectItem(root, "count");
    v_count = cJSON_GetNumberValue(fetched_count);
    detached = cJSON_DetachItemViaPointer(arr, fetched_item);

    // step 5: Validate - compute a simple validation score and free the detached item
    validation_score = (int)(v_item == 3.14) + (int)(v_count == 7.0) + (int)(detached != (cJSON *)0);
    cJSON_Delete(detached);

    // step 6: Cleanup - delete root which frees all remaining attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}