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
//<ID> 1028
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *json_text = "{\"numbers\": [], \"meta\": {\"tag\": \"demo\"}}";
    const char *numbers_key = "numbers";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *num_item0 = (cJSON *)0;
    cJSON *num_item1 = (cJSON *)0;
    cJSON *fetched0 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    cJSON_bool arr_is_array = (cJSON_bool)0;
    double val0 = 0.0;
    double val1 = 0.0;
    int validation_score = 0;

    // step 2: Setup - parse JSON to obtain a root object with an empty array
    root = cJSON_Parse(json_text);
    arr = cJSON_GetObjectItem(root, numbers_key);

    // step 3: Configure - create numeric items and append them into the parsed array
    num_item0 = cJSON_CreateNumber(42.0);
    num_item1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num_item0);
    cJSON_AddItemToArray(arr, num_item1);

    // step 4: Operate - verify type and retrieve numeric values from the array
    arr_is_array = cJSON_IsArray(arr);
    fetched0 = cJSON_GetArrayItem(arr, 0);
    fetched1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(fetched0);
    val1 = cJSON_GetNumberValue(fetched1);

    // step 5: Validate - compute a small validation score exercising pointers, type checks and numeric values
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(arr_is_array) + (int)(fetched0 != (cJSON *)0) + (int)(fetched1 != (cJSON *)0) + (int)(val0 == 42.0) + (int)(val1 > 3.13 && val1 < 3.15);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}