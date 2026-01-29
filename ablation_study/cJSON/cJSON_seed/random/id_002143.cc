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
//<ID> 2143
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
    cJSON *dup_arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *tru = NULL;
    cJSON *orig0 = NULL;
    cJSON *orig1 = NULL;
    cJSON *orig2 = NULL;
    cJSON *dup0 = NULL;
    cJSON *dup1 = NULL;
    cJSON *dup2 = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool is_true0 = 0;
    cJSON_bool dup_is_num0 = 0;
    cJSON_bool dup_is_true0 = 0;
    double val0 = 0.0;
    double val1 = 0.0;
    double dup_val0 = 0.0;
    int validation = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(10.0);
    n2 = cJSON_CreateNumber(20.5);
    tru = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, tru);
    dup_arr = cJSON_Duplicate(arr, 1);

    // step 3: Core operations
    orig0 = cJSON_GetArrayItem(arr, 0);
    orig1 = cJSON_GetArrayItem(arr, 1);
    orig2 = cJSON_GetArrayItem(arr, 2);
    dup0 = cJSON_GetArrayItem(dup_arr, 0);
    dup1 = cJSON_GetArrayItem(dup_arr, 1);
    dup2 = cJSON_GetArrayItem(dup_arr, 2);
    is_num0 = cJSON_IsNumber(orig0);
    is_num1 = cJSON_IsNumber(orig1);
    is_true0 = cJSON_IsTrue(orig2);
    dup_is_num0 = cJSON_IsNumber(dup0);
    dup_is_true0 = cJSON_IsTrue(dup2);
    val0 = cJSON_GetNumberValue(orig0);
    val1 = cJSON_GetNumberValue(orig1);
    dup_val0 = cJSON_GetNumberValue(dup0);

    // step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(dup_arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(tru != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_num1 != 0);
    validation ^= (int)(is_true0 != 0);
    validation ^= (int)(dup_is_num0 != 0);
    validation ^= (int)(dup_is_true0 != 0);
    validation ^= (int)(val0 == 10.0);
    validation ^= (int)(val1 == 20.5);
    validation ^= (int)(dup_val0 == 10.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(dup_arr);

    // API sequence test completed successfully
    return 66;
}