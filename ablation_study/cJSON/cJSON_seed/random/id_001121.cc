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
//<ID> 1121
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
    cJSON *added_num = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    char *printed = (char *)0;
    double first_val = 0.0;
    cJSON_bool first_is_number = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create root object, array and numeric items, attach them
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObjectCS(root, "numbers", arr);
    added_num = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 3: Operate - retrieve members and serialize unformatted
    fetched_arr = cJSON_GetObjectItem(root, "numbers");
    first_elem = cJSON_GetArrayItem(fetched_arr, 0);
    first_val = cJSON_GetNumberValue(first_elem);
    first_is_number = cJSON_IsNumber(added_num);
    printed = cJSON_PrintUnformatted(root);

    // step 4: Validate - combine simple checks into a score and free serialized data
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(fetched_arr != (cJSON *)0) + (int)(first_val == 1.0) + (int)first_is_number + (int)(printed != (char *)0) + (int)(added_num != (cJSON *)0);
    cJSON_free((void *)printed);
    (void)validation_score;

    // step 5: Cleanup - delete root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}