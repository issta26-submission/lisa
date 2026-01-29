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
//<ID> 1048
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *name_key = "name";
    const char *vals_key = "values";
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *num1 = (cJSON *)0;
    cJSON *num2 = (cJSON *)0;
    cJSON *fetched_arr = (cJSON *)0;
    cJSON *fetched_num0 = (cJSON *)0;
    int array_size = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON_bool added_num1 = (cJSON_bool)0;
    cJSON_bool added_num2 = (cJSON_bool)0;
    cJSON_bool added_name = (cJSON_bool)0;
    cJSON_bool added_array = (cJSON_bool)0;
    int validation_score = 0;

    // step 2: Setup - create object, array and items
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example");
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.5);

    // step 3: Configure - build array and attach to object using CS variant for object keys
    added_num1 = cJSON_AddItemToArray(arr, num1);
    added_num2 = cJSON_AddItemToArray(arr, num2);
    added_name = cJSON_AddItemToObjectCS(root, name_key, str_item);
    added_array = cJSON_AddItemToObjectCS(root, vals_key, arr);

    // step 4: Operate - fetch array, measure size, and retrieve numeric values
    fetched_arr = cJSON_GetObjectItem(root, vals_key);
    array_size = cJSON_GetArraySize(fetched_arr);
    fetched_num0 = cJSON_GetArrayItem(fetched_arr, 0);
    val0 = cJSON_GetNumberValue(fetched_num0);
    fetched_num0 = cJSON_GetArrayItem(fetched_arr, 1);
    val1 = cJSON_GetNumberValue(fetched_num0);

    // step 5: Validate - compute a small validation score exercising used APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(arr != (cJSON *)0) + (int)(str_item != (cJSON *)0) + (int)(num1 != (cJSON *)0) + (int)(num2 != (cJSON *)0) + (int)(added_num1) + (int)(added_num2) + (int)(added_name) + (int)(added_array) + (int)(array_size == 2) + (int)(val0 == 10.0) + (int)(val1 == 20.5);
    (void)validation_score;

    // step 6: Cleanup - delete root which frees attached children
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}