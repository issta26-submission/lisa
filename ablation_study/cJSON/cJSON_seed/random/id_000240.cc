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
//<ID> 240
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
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    cJSON *dup_item = (cJSON *)0;
    cJSON *obj_lookup = (cJSON *)0;
    int arr_size = 0;
    double first_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_AddArrayToObject(root, "numbers");

    // step 3: Configure
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 0);
    dup_item = cJSON_Duplicate(retrieved, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "first_element", dup_item);
    arr_size = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)arr_size);

    // step 5: Validate
    obj_lookup = cJSON_GetObjectItem(root, "first_element");
    first_value = cJSON_GetNumberValue(obj_lookup);
    cJSON_AddNumberToObject(root, "first_plus_count", first_value + (double)arr_size);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}