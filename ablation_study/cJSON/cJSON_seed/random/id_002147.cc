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
//<ID> 2147
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
    cJSON *dup_n0 = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_num_dup = 0;
    cJSON_bool is_true = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    n0 = cJSON_CreateNumber(1.5);
    n1 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, t);

    // step 3: Core operations
    dup_n0 = cJSON_Duplicate(n0, 0);
    is_num0 = cJSON_IsNumber(cJSON_GetArrayItem(arr, 0));
    is_num_dup = cJSON_IsNumber(dup_n0);
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    is_true = cJSON_IsTrue(cJSON_GetArrayItem(arr, 2));

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(t != NULL);
    validation ^= (int)(dup_n0 != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_num_dup != 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(v0 == 1.5);
    validation ^= (int)(v1 == 2.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(dup_n0);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}