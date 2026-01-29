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
//<ID> 639
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
    cJSON *num_item0 = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *retrieved0 = (cJSON *)0;
    cJSON *retrieved1 = (cJSON *)0;
    cJSON_bool added0 = 0;
    cJSON_bool added1 = 0;
    int array_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    double sum = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item0 = cJSON_CreateNumber(10.5);
    num_item1 = cJSON_CreateNumber(20.25);

    // step 3: Configure
    added0 = cJSON_AddItemToArray(arr, num_item0);
    added1 = cJSON_AddItemToArray(arr, num_item1);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    array_size = cJSON_GetArraySize(arr);
    retrieved0 = cJSON_GetArrayItem(arr, 0);
    retrieved1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(retrieved0);
    val1 = cJSON_GetNumberValue(retrieved1);
    sum = val0 + val1;

    // step 5: Validate
    (void)added0;
    (void)added1;
    (void)array_size;
    (void)val0;
    (void)val1;
    (void)retrieved0;
    (void)retrieved1;
    (void)sum;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}