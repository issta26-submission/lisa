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
//<ID> 1677
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
    cJSON *detached_arr = (cJSON *)0;
    cJSON *num0 = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *bool_false = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_false = 0;
    double first_val = 0.0;
    int validation = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.71);
    bool_false = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, bool_false);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate
    detached_arr = cJSON_DetachItemFromObjectCaseSensitive(root, "payload");
    first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(detached_arr, 0));
    is_false = cJSON_IsFalse(cJSON_GetArrayItem(detached_arr, 2));
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation = (int)(first_val == 3.14) + (int)is_false + (int)(detached_arr != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation;

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(detached_arr);
    (void)root;
    (void)arr;
    (void)detached_arr;
    (void)num0;
    (void)num1;
    (void)bool_false;
    (void)first_val;
    (void)is_false;
    // API sequence test completed successfully
    return 66;
}