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
//<ID> 635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *arr = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int size = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    double sum = 0.0;

    // step 2: Initialize
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.0);

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, num1);
    added2 = cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    size = cJSON_GetArraySize(arr);
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    retrieved1 = cJSON_GetArrayItem(arr, 1);
    v0 = cJSON_GetNumberValue(retrieved0);
    v1 = cJSON_GetNumberValue(retrieved1);
    sum = v0 + v1;

    // step 5: Validate
    (void)added1;
    (void)added2;
    (void)size;
    (void)retrieved0;
    (void)retrieved1;
    (void)v0;
    (void)v1;
    (void)sum;

    // step 6: Cleanup
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}