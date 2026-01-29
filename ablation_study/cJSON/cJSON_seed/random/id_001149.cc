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
//<ID> 1149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *child = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *fetched_child = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_elem0 = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double extracted_number = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and child object
    root = cJSON_CreateObject();
    child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 3: Configure - create an array with a numeric element and a separate string member
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "values", arr);
    num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_item);
    str_item = cJSON_CreateString("detachable-string");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate - retrieve nested object and array element, extract number, detach the string item
    fetched_child = cJSON_GetObjectItem(root, "child");
    fetched_arr = cJSON_GetObjectItem(fetched_child, "values");
    fetched_elem0 = cJSON_GetArrayItem(fetched_arr, 0);
    extracted_number = cJSON_GetNumberValue(fetched_elem0);
    detached = cJSON_DetachItemViaPointer(root, str_item);

    // step 5: Validate - compute a simple validation score using retrieved data
    validation_score = (int)(fetched_child != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(extracted_number == 3.14) + (int)(detached != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - delete detached item and delete root to free remaining items
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}