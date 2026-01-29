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
//<ID> 2021
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *sum_item = NULL;
    cJSON *label_item = NULL;
    cJSON *got_n1 = NULL;
    cJSON *got_sum = NULL;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added_arr = 0;
    double v_n1 = 0.0;
    double v_sum = 0.0;
    double computed_total = 0.0;
    int validation = 0;

    // step 2: Setup (create root, array and numeric items)
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.0);

    // step 3: Configure (populate array and object with number and string)
    added1 = cJSON_AddItemToArray(arr, n1);
    added2 = cJSON_AddItemToArray(arr, n2);
    added_arr = cJSON_AddItemToObject(root, "items", arr);
    sum_item = cJSON_AddNumberToObject(root, "sum", 30.0);
    label_item = cJSON_AddStringToObject(root, "label", "example");

    // step 4: Operate (retrieve values and compute)
    got_n1 = cJSON_GetArrayItem(arr, 0);
    v_n1 = cJSON_GetNumberValue(got_n1);
    got_sum = cJSON_GetObjectItem(root, "sum");
    v_sum = cJSON_GetNumberValue(got_sum);
    computed_total = v_n1 + v_sum;

    // step 5: Validate (aggregate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(added1 != 0);
    validation ^= (int)(added2 != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(sum_item != NULL);
    validation ^= (int)(label_item != NULL);
    validation ^= (int)(got_n1 != NULL);
    validation ^= (int)(got_sum != NULL);
    validation ^= (int)(v_n1 == 10.0);
    validation ^= (int)(v_sum == 30.0);
    validation ^= (int)(computed_total == 40.0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}