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
//<ID> 1300
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
    cJSON *flag_node = (cJSON *)0;
    cJSON *str_node = (cJSON *)0;
    cJSON *retrieved_flag = (cJSON *)0;
    cJSON *retrieved_str = (cJSON *)0;
    int array_size = 0;
    cJSON_bool flag_is_false = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    num3 = cJSON_CreateNumber(3.0);
    flag_node = cJSON_CreateFalse();

    // step 3: Configure - attach numbers to array and attach array, flag and a string to root
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToArray(arr, num3);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON_AddItemToObject(root, "flag", flag_node);
    str_node = cJSON_AddStringToObject(root, "greeting", "hello");

    // step 4: Operate - retrieve items case-sensitively and inspect types/values
    retrieved_flag = cJSON_GetObjectItemCaseSensitive(root, "flag");
    retrieved_str = cJSON_GetObjectItemCaseSensitive(root, "greeting");
    array_size = cJSON_GetArraySize(cJSON_GetObjectItemCaseSensitive(root, "list"));
    flag_is_false = cJSON_IsFalse(retrieved_flag);

    // step 5: Validate - compute a simple validation score from the results
    validation_score = (int)(retrieved_flag != (cJSON *)0) + (int)(retrieved_str != (cJSON *)0) + (int)(array_size == 3) + (int)(flag_is_false != 0);
    (void)validation_score;

    // step 6: Cleanup - delete the root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}