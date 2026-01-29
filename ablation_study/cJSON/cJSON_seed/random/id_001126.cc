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
//<ID> 1126
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
    cJSON *titem = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_item = (cJSON *)0;
    char *printed = (char *)0;
    double retrieved_value = 0.0;
    cJSON_bool is_number = (cJSON_bool)0;
    int validation_score = 0;
    const char *cs_key = "numbers";

    // step 2: Setup - create root object, create array and number items, attach array using CS API
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObjectCS(root, cs_key, arr);
    titem = cJSON_AddNumberToObject(root, "total", 3.0);

    // step 3: Operate - fetch the array and its first element, inspect value and type
    fetched_arr = cJSON_GetObjectItem(root, cs_key);
    fetched_item = cJSON_GetArrayItem(fetched_arr, 0);
    retrieved_value = cJSON_GetNumberValue(fetched_item);
    is_number = cJSON_IsNumber(fetched_item);

    // step 4: Serialize - produce compact JSON and free the buffer
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - compute a simple validation score from meaningful checks
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(titem != (cJSON *)0) + (int)is_number + (int)(retrieved_value == 1.0);

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}