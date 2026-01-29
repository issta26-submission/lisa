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
//<ID> 2148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // Step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *t = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *got2 = NULL;
    cJSON *dup = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool is_dup_num = 0;
    cJSON_bool is_t2 = 0;
    double v0 = 0.0;
    double vdup = 0.0;
    int validation = 0;

    // Step 2: Setup
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(3.0);
    n2 = cJSON_CreateNumber(5.0);
    t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);

    // Step 3: Core operations
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    got2 = cJSON_GetArrayItem(arr, 2);
    is_num0 = cJSON_IsNumber(got0);
    is_num1 = cJSON_IsNumber(got1);
    is_t2 = cJSON_IsTrue(got2);
    dup = cJSON_Duplicate(got1, 1);
    is_dup_num = cJSON_IsNumber(dup);
    v0 = cJSON_GetNumberValue(got0);
    vdup = cJSON_GetNumberValue(dup);
    cJSON_AddItemToArray(arr, dup);

    // Step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(t != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(got2 != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_num1 != 0);
    validation ^= (int)(is_dup_num != 0);
    validation ^= (int)(is_t2 != 0);
    validation ^= (int)(v0 == 3.0);
    validation ^= (int)(vdup == 5.0);
    buf[0] = (char)(validation + '0');

    // Step 5: Cleanup
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}