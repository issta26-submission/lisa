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
//<ID> 572
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
    cJSON *objA = (cJSON *)0;
    cJSON *objB = (cJSON *)0;
    cJSON *inner_num = (cJSON *)0;
    cJSON *inner_repl = (cJSON *)0;
    int arr_size = 0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool replace_ok1 = 0;
    cJSON_bool replace_ok2 = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    objA = cJSON_CreateObject();
    objB = cJSON_CreateObject();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(arr, num1);
    add_ok2 = cJSON_AddItemToArray(arr, num2);
    add_ok3 = cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddNumberToObject(objA, "value", 10.0);
    add_ok3 = cJSON_AddItemToObject(root, "child", objA);

    // step 4: Operate
    cJSON_AddStringToObject(objB, "status", "new");
    replace_ok1 = cJSON_ReplaceItemInObject(root, "child", objB);
    inner_num = cJSON_CreateNumber(99.0);
    cJSON_AddItemToObject(objB, "inner", inner_num);
    inner_repl = cJSON_CreateString("replaced");
    replace_ok2 = cJSON_ReplaceItemViaPointer(objB, inner_num, inner_repl);

    // step 5: Validate
    arr_size = cJSON_GetArraySize(arr);
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)replace_ok1;
    (void)replace_ok2;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}