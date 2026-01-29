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
//<ID> 2090
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_item0 = NULL;
    cJSON *num_item1 = NULL;
    cJSON *raw_item = NULL;
    cJSON *got_scalar = NULL;
    cJSON *got_values = NULL;
    cJSON *arr0 = NULL;
    cJSON *arr1 = NULL;
    cJSON *arr2 = NULL;
    double scalar_val = 0.0;
    double num0_val = 0.0;
    double num1_val = 0.0;
    cJSON_bool raw_check = 0;
    cJSON_bool isnum0 = 0;
    cJSON_bool isnum1 = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item0 = cJSON_CreateNumber(3.14);
    num_item1 = cJSON_CreateNumber(2.718);
    raw_item = cJSON_CreateRaw("raw_payload");
    cJSON_AddItemToArray(arr, num_item0);
    cJSON_AddItemToArray(arr, num_item1);
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "scalar", 42.0);

    // step 3: Operate
    got_scalar = cJSON_GetObjectItem(root, "scalar");
    got_values = cJSON_GetObjectItem(root, "values");
    arr0 = cJSON_GetArrayItem(got_values, 0);
    arr1 = cJSON_GetArrayItem(got_values, 1);
    arr2 = cJSON_GetArrayItem(got_values, 2);
    scalar_val = cJSON_GetNumberValue(got_scalar);
    num0_val = cJSON_GetNumberValue(arr0);
    num1_val = cJSON_GetNumberValue(arr1);
    raw_check = cJSON_IsRaw(arr2);
    isnum0 = cJSON_IsNumber(arr0);
    isnum1 = cJSON_IsNumber(arr2);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(got_scalar != NULL);
    validation ^= (int)(got_values != NULL);
    validation ^= (int)(arr0 != NULL);
    validation ^= (int)(arr1 != NULL);
    validation ^= (int)(arr2 != NULL);
    validation ^= (int)(scalar_val == 42.0);
    validation ^= (int)(num0_val == 3.14);
    validation ^= (int)(num1_val == 2.718);
    validation ^= (int)(raw_check != 0);
    validation ^= (int)(isnum0 != 0);
    validation ^= (int)(isnum1 == 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}