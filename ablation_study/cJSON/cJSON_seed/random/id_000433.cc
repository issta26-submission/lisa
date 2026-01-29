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
//<ID> 433
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
    cJSON *got1 = (cJSON *)0;
    cJSON *flag_true = (cJSON *)0;
    cJSON *doubled = (cJSON *)0;
    double extracted = 0.0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    cJSON_bool added_arr_to_obj = 0;
    cJSON_bool added_flag = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(10.5);
    n1 = cJSON_CreateNumber(2.5);

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, n0);
    added1 = cJSON_AddItemToArray(arr, n1);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);
    flag_true = cJSON_CreateTrue();
    added_flag = cJSON_AddItemToObject(root, "enabled", flag_true);

    // step 4: Operate
    got1 = cJSON_GetArrayItem(arr, 1);
    extracted = cJSON_GetNumberValue(got1);
    doubled = cJSON_CreateNumber(extracted * 2.0);
    cJSON_AddItemToObject(root, "doubled", doubled);

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)added_arr_to_obj;
    (void)added_flag;
    (void)extracted;
    (void)got1;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}