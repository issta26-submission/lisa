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
//<ID> 555
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
    cJSON *num3 = (cJSON *)0;
    cJSON *bool_false = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool add_ok5 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(3.14);
    num2 = cJSON_CreateNumber(2.718);
    num3 = cJSON_CreateNumber(-1.0);
    bool_false = cJSON_CreateFalse();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    add_ok3 = cJSON_AddItemToArray(arr, num3);
    add_ok4 = cJSON_AddItemToArray(arr, bool_false);
    add_ok5 = cJSON_AddItemToObject(root, "values", arr);

    // step 4: Operate
    retrieved = cJSON_GetArrayItem(arr, 0);
    val0 = cJSON_GetNumberValue(retrieved);
    retrieved = cJSON_GetArrayItem(arr, 1);
    val1 = cJSON_GetNumberValue(retrieved);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)add_ok5;
    (void)val0;
    (void)val1;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}