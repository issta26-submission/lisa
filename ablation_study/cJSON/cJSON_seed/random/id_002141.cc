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
//<ID> 2141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *arr = NULL;
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *t = NULL;
    cJSON *got_n0 = NULL;
    cJSON *got_n1 = NULL;
    cJSON *dup_n0 = NULL;
    cJSON *dup_arr = NULL;
    cJSON *dup_arr_n0 = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool is_true = 0;
    cJSON_bool is_dup_num0 = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    double vdup0 = 0.0;
    int validation = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.23);
    n1 = cJSON_CreateNumber(4.56);
    t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, t);

    // step 3: Core operations
    got_n0 = cJSON_GetArrayItem(arr, 0);
    got_n1 = cJSON_GetArrayItem(arr, 1);
    is_num0 = cJSON_IsNumber(got_n0);
    is_num1 = cJSON_IsNumber(got_n1);
    is_true = cJSON_IsTrue(cJSON_GetArrayItem(arr, 2));
    v0 = cJSON_GetNumberValue(got_n0);
    v1 = cJSON_GetNumberValue(got_n1);
    dup_n0 = cJSON_Duplicate(got_n0, 0);
    dup_arr = cJSON_Duplicate(arr, 1);
    dup_arr_n0 = cJSON_GetArrayItem(dup_arr, 0);
    is_dup_num0 = cJSON_IsNumber(dup_arr_n0);
    vdup0 = cJSON_GetNumberValue(dup_arr_n0);

    // step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(t != NULL);
    validation ^= (int)(got_n0 != NULL);
    validation ^= (int)(got_n1 != NULL);
    validation ^= (int)(dup_n0 != NULL);
    validation ^= (int)(dup_arr != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_num1 != 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(is_dup_num0 != 0);
    validation ^= (int)(v0 == 1.23);
    validation ^= (int)(v1 == 4.56);
    validation ^= (int)(vdup0 == 1.23);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(dup_arr);
    cJSON_Delete(dup_n0);

    // API sequence test completed successfully
    return 66;
}