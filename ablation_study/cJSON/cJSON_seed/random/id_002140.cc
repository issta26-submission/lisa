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
//<ID> 2140
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
    cJSON *num = NULL;
    cJSON *tru = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *dup_num = NULL;
    cJSON_bool is_num = 0;
    cJSON_bool is_true = 0;
    double val_dup = 0.0;
    int validation = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    num = cJSON_CreateNumber(42.0);
    tru = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, num);
    cJSON_AddItemToArray(arr, tru);

    // step 3: Core operations
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    is_num = cJSON_IsNumber(got0);
    is_true = cJSON_IsTrue(got1);
    dup_num = cJSON_Duplicate(got0, 1);
    val_dup = cJSON_GetNumberValue(dup_num);

    // step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num != NULL);
    validation ^= (int)(tru != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(dup_num != NULL);
    validation ^= (int)(is_num != 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(val_dup == 42.0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(arr);
    cJSON_Delete(dup_num);

    // API sequence test completed successfully
    return 66;
}