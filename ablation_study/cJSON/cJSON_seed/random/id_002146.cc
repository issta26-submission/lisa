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
//<ID> 2146
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *dup_n1 = NULL;
    cJSON *got0 = NULL;
    cJSON *got2 = NULL;
    cJSON_bool is_num0 = 0;
    cJSON_bool is_true2 = 0;
    double v0 = 0.0;
    double vdup = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    n1 = cJSON_CreateNumber(123.456);
    n2 = cJSON_CreateNumber(78.9);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, cJSON_CreateTrue());

    // step 3: Core operations
    dup_n1 = cJSON_Duplicate(n1, 1);
    got0 = cJSON_GetArrayItem(arr, 0);
    got2 = cJSON_GetArrayItem(arr, 2);
    is_num0 = cJSON_IsNumber(got0);
    is_true2 = cJSON_IsTrue(got2);
    v0 = cJSON_GetNumberValue(got0);
    vdup = cJSON_GetNumberValue(dup_n1);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(dup_n1 != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got2 != NULL);
    validation ^= (int)(is_num0 != 0);
    validation ^= (int)(is_true2 != 0);
    validation ^= (int)(v0 == 123.456);
    validation ^= (int)(vdup == 123.456);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(dup_n1);

    // API sequence test completed successfully
    return 66;
}