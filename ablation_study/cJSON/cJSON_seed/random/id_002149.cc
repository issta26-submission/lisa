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
//<ID> 2149
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
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *t = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *dup_n0 = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num1 = 0;
    cJSON_bool is_t = 0;
    double v_dup0 = 0.0;
    double v1 = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    n0 = cJSON_CreateNumber(10.0);
    n1 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, t);

    // step 3: Operate
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    is_num0 = cJSON_IsNumber(got0);
    is_num1 = cJSON_IsNumber(got1);
    dup_n0 = cJSON_Duplicate(got0, 1);
    v_dup0 = cJSON_GetNumberValue(dup_n0);
    v1 = cJSON_GetNumberValue(got1);
    is_t = cJSON_IsTrue(cJSON_GetArrayItem(arr, 2));
    buf[0] = (char)(is_t + '0');

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(dup_n0 != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_num1 != 0);
    validation ^= (int)(is_t != 0);
    validation ^= (int)(v_dup0 == 10.0);
    validation ^= (int)(v1 == 20.0);
    buf[1] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_n0);

    // API sequence test completed successfully
    return 66;
}