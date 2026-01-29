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
//<ID> 1024
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"numbers\": [1, 2]}";
    const char *numbers_key = "numbers";
    cJSON *root = (cJSON *)0;
    cJSON *numbers_arr = (cJSON *)0;
    cJSON *new_num1 = (cJSON *)0;
    cJSON *new_num2 = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched2 = (cJSON *)0;
    cJSON_bool add_ok1 = (cJSON_bool)0;
    cJSON_bool add_ok2 = (cJSON_bool)0;
    double val0 = 0.0;
    double val2 = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse initial JSON and obtain the numbers array
    root = cJSON_Parse(json_text);
    numbers_arr = cJSON_GetObjectItem(root, numbers_key);

    // step 3: Configure - create new numeric items and append them to the array
    new_num1 = cJSON_CreateNumber(3.5);
    new_num2 = cJSON_CreateNumber(4.5);
    add_ok1 = cJSON_AddItemToArray(numbers_arr, new_num1);
    add_ok2 = cJSON_AddItemToArray(numbers_arr, new_num2);

    // step 4: Operate - fetch elements and read numeric values
    fetched0 = cJSON_GetArrayItem(numbers_arr, 0);
    fetched2 = cJSON_GetArrayItem(numbers_arr, 2);
    val0 = cJSON_GetNumberValue(fetched0);
    val2 = cJSON_GetNumberValue(fetched2);

    // step 5: Validate - combine pointer checks and type/value checks into a score
    validation_score = (int)(root != (cJSON *)0)
                     + (int)(numbers_arr != (cJSON *)0)
                     + (int)(cJSON_IsArray(numbers_arr))
                     + (int)(new_num1 != (cJSON *)0)
                     + (int)(new_num2 != (cJSON *)0)
                     + (int)(add_ok1)
                     + (int)(add_ok2)
                     + (int)(fetched0 != (cJSON *)0)
                     + (int)(fetched2 != (cJSON *)0)
                     + (int)(val0 == 1.0)
                     + (int)(val2 == 3.5);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}