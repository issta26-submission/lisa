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
//<ID> 1303
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
    cJSON *retrieved_name = (cJSON *)0;
    cJSON *flag_item = (cJSON *)0;
    const char *name_str = (const char *)0;
    int arr_size = 0;
    int validation_score = 0;
    cJSON_bool flag_is_false = (cJSON_bool)0;

    // step 2: Initialize - create root object, an array and numeric items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);

    // step 3: Configure - attach array and other items to the root
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddStringToObject(root, "name", "Charlie");
    cJSON_AddItemToObject(root, "enabled", cJSON_CreateFalse());

    // step 4: Operate - retrieve items case-sensitively, inspect boolean and array size
    retrieved_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_str = cJSON_GetStringValue(retrieved_name);
    flag_item = cJSON_GetObjectItemCaseSensitive(root, "enabled");
    flag_is_false = cJSON_IsFalse(flag_item);
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate - derive a simple validation score from retrieved data
    validation_score = (int)(retrieved_name != (cJSON *)0) + (int)(name_str != (const char *)0) + (int)flag_is_false + arr_size;
    (void)validation_score;
    
    // step 6: Cleanup - delete root to free all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}