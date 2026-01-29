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
//<ID> 2124
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char *scratch = (char *)cJSON_malloc(16);
    memset(scratch, 0, 16);
    cJSON *root = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *flag = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON *gotflag = NULL;
    double v0 = 0.0;
    double v1 = 0.0;
    cJSON_bool is_true = 0;
    int size = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(3.14);
    num1 = cJSON_CreateNumber(2.0);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToArray(root, num0);
    cJSON_AddItemToArray(root, num1);
    cJSON_AddItemToArray(root, flag);

    // step 3: Operate
    got0 = cJSON_GetArrayItem(root, 0);
    got1 = cJSON_GetArrayItem(root, 1);
    gotflag = cJSON_GetArrayItem(root, 2);
    v0 = cJSON_GetNumberValue(got0);
    v1 = cJSON_GetNumberValue(got1);
    is_true = cJSON_IsTrue(gotflag);
    size = cJSON_GetArraySize(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num0 != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(flag != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(gotflag != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(v0 == 3.14);
    validation ^= (int)(v1 == 2.0);
    validation ^= (int)(size == 3);
    scratch[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}