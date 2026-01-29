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
//<ID> 432
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
    cJSON *arr_item = (cJSON *)0;
    cJSON *tripled = (cJSON *)0;
    cJSON *flag_true = (cJSON *)0;
    char *printed = (char *)0;
    double value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(12.5);
    n1 = cJSON_CreateNumber(4.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    arr_item = cJSON_GetArrayItem(arr, 0);
    value = cJSON_GetNumberValue(arr_item);
    tripled = cJSON_CreateNumber(value * 3.0);
    cJSON_AddItemToObject(root, "tripled_first", tripled);

    // step 5: Validate
    flag_true = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "present", flag_true);

    // step 6: Cleanup
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}