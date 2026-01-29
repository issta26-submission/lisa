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
//<ID> 491
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
    cJSON *num_item = (cJSON *)0;
    cJSON *added_num = (cJSON *)0;
    cJSON *first_in_array = (cJSON *)0;
    char buffer[256];
    cJSON_bool printed_ok = 0;
    double retrieved_val = 0.0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(42.0);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3.14));
    cJSON_AddItemToObject(root, "numbers", arr);
    added_num = cJSON_AddNumberToObject(root, "pi", 3.1415);

    // step 4: Operate
    memset(buffer, 0, sizeof(buffer));
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    first_in_array = cJSON_GetArrayItem(arr, 0);
    retrieved_val = cJSON_GetNumberValue(added_num);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    (void)printed_ok;
    (void)retrieved_val;
    (void)first_in_array;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}