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
//<ID> 419
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
    cJSON *n0 = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n_replace = (cJSON *)0;
    cJSON *arr_item = (cJSON *)0;
    cJSON *label = (cJSON *)0;
    cJSON *doubled = (cJSON *)0;
    double extracted = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(3.14159);
    n1 = cJSON_CreateNumber(6.28318);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    label = cJSON_AddStringToObject(root, "label", "pi-multiples");

    // step 4: Operate
    n_replace = cJSON_CreateNumber(9.42477);
    cJSON_ReplaceItemInArray(arr, 1, n_replace);
    arr_item = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(arr_item);

    // step 5: Validate
    doubled = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToObject(root, "doubled", doubled);
    (void)label;
    (void)extracted;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}