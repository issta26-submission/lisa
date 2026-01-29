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
//<ID> 550
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
    cJSON *num_a = (cJSON *)0;
    cJSON *num_b = (cJSON *)0;
    cJSON *num_c = (cJSON *)0;
    cJSON *flag = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    double v0 = 0.0;
    double v1 = 0.0;
    double v2 = 0.0;
    double sum = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool add_ok5 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(10.5);
    num_b = cJSON_CreateNumber(20.25);
    num_c = cJSON_CreateNumber(-3.75);
    flag = cJSON_CreateFalse();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num_a);
    add_ok2 = cJSON_AddItemToArray(arr, num_b);
    add_ok3 = cJSON_AddItemToArray(arr, num_c);
    add_ok4 = cJSON_AddItemToObject(root, "values", arr);
    add_ok5 = cJSON_AddItemToObject(root, "flag", flag);

    // step 4: Operate
    item0 = cJSON_GetArrayItem(arr, 0);
    item1 = cJSON_GetArrayItem(arr, 1);
    item2 = cJSON_GetArrayItem(arr, 2);
    v0 = cJSON_GetNumberValue(item0);
    v1 = cJSON_GetNumberValue(item1);
    v2 = cJSON_GetNumberValue(item2);
    sum = v0 + v1 + v2;

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)add_ok5;
    (void)v0;
    (void)v1;
    (void)v2;
    (void)sum;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}