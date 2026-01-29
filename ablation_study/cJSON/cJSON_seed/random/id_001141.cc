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
//<ID> 1141
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
    cJSON *name0 = (cJSON *)0;
    cJSON *name1 = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON *fetched_num = (cJSON *)0;
    cJSON *fetched_elem = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    double value_count = 0.0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array, attach array to root
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "people", arr);

    // step 3: Configure - create string items and a numeric item, populate array and root
    name0 = cJSON_CreateString("Alice");
    name1 = cJSON_CreateString("Bob");
    cJSON_AddItemToArray(arr, name0);
    cJSON_AddItemToArray(arr, name1);
    num_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Operate - retrieve numeric object member and array element, detach an element via pointer
    fetched_num = cJSON_GetObjectItem(root, "count");
    value_count = cJSON_GetNumberValue(fetched_num);
    fetched_elem = cJSON_GetArrayItem(arr, 1);
    detached = cJSON_DetachItemViaPointer(arr, fetched_elem);
    cJSON_Delete(detached);

    // step 5: Validate - compute a simple validation score and cleanup
    validation_score = (int)(value_count == 2.0) + (int)(cJSON_GetArrayItem(arr, 0) != (cJSON *)0);
    (void)validation_score;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}