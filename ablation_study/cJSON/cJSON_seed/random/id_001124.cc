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
//<ID> 1124
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
    cJSON *first_num = (cJSON *)0;
    cJSON *second_num = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_first = (cJSON *)0;
    cJSON *fetched_second = (cJSON *)0;
    cJSON *fetched_total = (cJSON *)0;
    char *printed = (char *)0;
    double val_first = 0.0;
    double val_second = 0.0;
    double val_total = 0.0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    cJSON_bool second_is_number = (cJSON_bool)0;
    cJSON_bool total_is_number = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and an array
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure - populate the array and attach to root using AddItemToObjectCS; also add a direct number
    first_num = cJSON_CreateNumber(10.0);
    second_num = cJSON_CreateNumber(20.5);
    cJSON_AddItemToArray(arr, first_num);
    cJSON_AddItemToArray(arr, second_num);
    cJSON_AddItemToObjectCS(root, "values_cs", arr);
    cJSON_AddNumberToObject(root, "total", 30.5);

    // step 4: Operate - retrieve members and array elements, inspect values, serialize
    fetched_arr = cJSON_GetObjectItem(root, "values_cs");
    fetched_first = cJSON_GetArrayItem(fetched_arr, 0);
    fetched_second = cJSON_GetArrayItem(fetched_arr, 1);
    fetched_total = cJSON_GetObjectItem(root, "total");
    val_first = cJSON_GetNumberValue(fetched_first);
    val_second = cJSON_GetNumberValue(fetched_second);
    val_total = cJSON_GetNumberValue(fetched_total);
    first_is_number = cJSON_IsNumber(fetched_first);
    second_is_number = cJSON_IsNumber(fetched_second);
    total_is_number = cJSON_IsNumber(fetched_total);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free((void *)printed);

    // step 5: Validate - combine checks into a score (no branches)
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)first_is_number + (int)second_is_number + (int)total_is_number + (int)(val_first == 10.0) + (int)(val_second == 20.5) + (int)(val_total == 30.5);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}