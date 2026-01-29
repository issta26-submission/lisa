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
//<ID> 2029
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num_a = NULL;
    cJSON *num_b = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *sum_item = NULL;
    double val0 = 0.0;
    double val1 = 0.0;
    double val_sum = 0.0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    cJSON_bool added_arr_to_obj = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_a = cJSON_CreateNumber(1.5);
    num_b = cJSON_CreateNumber(2.5);

    // step 3: Configure
    added_a = cJSON_AddItemToArray(arr, num_a);
    added_b = cJSON_AddItemToArray(arr, num_b);
    added_arr_to_obj = cJSON_AddItemToObject(root, "values", arr);
    cJSON_AddNumberToObject(root, "sum", 4.0);
    cJSON_AddStringToObject(root, "label", "cjson_test");

    // step 4: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    val0 = cJSON_GetNumberValue(got0);
    val1 = cJSON_GetNumberValue(got1);
    sum_item = cJSON_GetObjectItem(root, "sum");
    val_sum = cJSON_GetNumberValue(sum_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num_a != NULL);
    validation ^= (int)(num_b != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(added_a != 0);
    validation ^= (int)(added_b != 0);
    validation ^= (int)(added_arr_to_obj != 0);
    validation ^= (int)(val0 == 1.5);
    validation ^= (int)(val1 == 2.5);
    validation ^= (int)(val_sum == 4.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}