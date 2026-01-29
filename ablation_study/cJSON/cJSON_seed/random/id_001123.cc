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
//<ID> 1123
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
    cJSON *num_in_object = (cJSON *)0;
    cJSON *num_in_array1 = (cJSON *)0;
    cJSON *num_in_array2 = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_first = (cJSON *)0;
    cJSON *fetched_value = (cJSON *)0;
    char *printed = (char *)0;
    double first_val = 0.0;
    double object_val = 0.0;
    cJSON_bool arr_is_array = (cJSON_bool)0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object and array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - populate array and add numeric member to root
    num_in_array1 = cJSON_CreateNumber(1.0);
    num_in_array2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num_in_array1);
    cJSON_AddItemToArray(arr, num_in_array2);
    num_in_object = cJSON_AddNumberToObject(root, "value", 3.14);

    // step 4: Attach array to root using case-sensitive add
    cJSON_AddItemToObjectCS(root, "arrCS", arr);

    // step 5: Operate & Validate - fetch items, inspect types and values, serialize
    fetched_arr = cJSON_GetObjectItem(root, "arrCS");
    arr_is_array = cJSON_IsArray(fetched_arr);
    fetched_first = cJSON_GetArrayItem(fetched_arr, 0);
    first_is_number = cJSON_IsNumber(fetched_first);
    first_val = cJSON_GetNumberValue(fetched_first);
    fetched_value = cJSON_GetObjectItem(root, "value");
    object_val = cJSON_GetNumberValue(fetched_value);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)arr_is_array + (int)first_is_number + (int)(first_val == 1.0) + (int)(object_val == 3.14);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}