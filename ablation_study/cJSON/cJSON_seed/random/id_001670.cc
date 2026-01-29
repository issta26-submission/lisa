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
//<ID> 1670
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
    cJSON *false_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_false = 0;
    double first_value = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.5);
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, false_item);
    cJSON_AddItemToObject(root, "list", arr);

    // step 4: Operate
    first_value = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    is_false = cJSON_IsFalse(cJSON_GetArrayItem(arr, 1));
    detached = cJSON_DetachItemFromObjectCaseSensitive(root, "list");
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(first_value == 1.5) + (int)is_false + (int)(detached != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    (void)root;
    (void)arr;
    (void)num0;
    (void)false_item;
    (void)detached;
    (void)printed;
    (void)is_false;
    (void)first_value;

    // API sequence test completed successfully
    return 66;
}