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
//<ID> 499
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
    cJSON *first = (cJSON *)0;
    double val_first = 0.0;
    double val_scalar = 0.0;
    char buffer[256];
    cJSON_bool add_ok = 0;
    cJSON_bool printed_ok = 0;
    memset(buffer, 0, sizeof(buffer));

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(arr, num1);
    num2 = cJSON_AddNumberToObject(root, "scalar", 3.14);
    add_ok = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    val_first = cJSON_GetNumberValue(first);
    val_scalar = cJSON_GetNumberValue(num2);

    // step 5: Validate
    printed_ok = cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 1);
    (void)val_first;
    (void)val_scalar;
    (void)printed_ok;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}