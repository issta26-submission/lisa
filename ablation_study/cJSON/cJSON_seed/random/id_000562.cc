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
//<ID> 562
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
    cJSON *first_elem = (cJSON *)0;
    char *printed = (char *)0;
    double first_value = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.5);
    num2 = cJSON_CreateNumber(-3.14);

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "numbers");
    first_elem = cJSON_GetArrayItem(retrieved, 0);
    first_value = cJSON_GetNumberValue(first_elem);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)first_value;
    (void)printed;
    (void)retrieved;
    (void)first_elem;

    // step 6: Cleanup
    if (printed) { cJSON_free(printed); }
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}