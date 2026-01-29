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
//<ID> 2145
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *t = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *got2 = NULL;
    cJSON *dup_n1 = NULL;
    cJSON *dup_arr = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool is_true = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.5);
    t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);

    // step 3: Core operations
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    got2 = cJSON_GetArrayItem(arr, 2);
    is_num0 = cJSON_IsNumber(got0);
    is_num1 = cJSON_IsNumber(got1);
    is_true = cJSON_IsTrue(got2);
    v0 = cJSON_GetNumberValue(got0);
    v1 = cJSON_GetNumberValue(got1);
    dup_n1 = cJSON_Duplicate(got1, 1);
    dup_arr = cJSON_Duplicate(arr, 1);
    got1 = cJSON_GetArrayItem(dup_arr, 1);

    // step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(t != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_num1 != 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(v0 == 10.0);
    validation ^= (int)(v1 == 20.5);
    validation ^= (int)(dup_n1 != NULL);
    validation ^= (int)(dup_arr != NULL);
    validation ^= (int)(got1 != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(dup_arr);
    cJSON_Delete(dup_n1);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}