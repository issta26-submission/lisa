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
//<ID> 490
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
    cJSON *num_in_root = (cJSON *)0;
    cJSON *num_in_arr = (cJSON *)0;
    char *buffer = (char *)0;
    cJSON_bool printed_ok = 0;
    double extracted_value = 0.0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_in_root = cJSON_AddNumberToObject(root, "root_number", 123.0);
    num_in_arr = cJSON_CreateNumber(9.5);

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_in_arr);
    cJSON_AddItemToObject(root, "numbers", arr);

    // step 4: Operate
    buffer = (char *)cJSON_malloc((size_t)256);
    memset(buffer, 0, (size_t)256);
    printed_ok = cJSON_PrintPreallocated(root, buffer, 256, 1);

    // step 5: Validate
    extracted_value = cJSON_GetNumberValue(num_in_root);
    (void)extracted_value;
    (void)printed_ok;

    // step 6: Cleanup
    cJSON_free((void *)buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}