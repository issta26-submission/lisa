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
//<ID> 371
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
    cJSON *got_array = (cJSON *)0;
    cJSON *array_item = (cJSON *)0;
    cJSON *detached_item = (cJSON *)0;
    const char *key = "values";
    double numbers[4] = {1.1, 2.2, 3.3, 4.4};
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateDoubleArray(numbers, 4);

    // step 3: Configure
    cJSON_AddItemToObject(root, key, arr);

    // step 4: Operate
    got_array = cJSON_GetObjectItem(root, key);
    array_item = cJSON_GetArrayItem(got_array, 2);
    detached_item = cJSON_DetachItemFromArray(got_array, 2);
    extracted_value = cJSON_GetNumberValue(detached_item);

    // step 5: Validate
    (void)array_item;
    (void)extracted_value;
    (void)got_array;

    // step 6: Cleanup
    cJSON_Delete(detached_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}