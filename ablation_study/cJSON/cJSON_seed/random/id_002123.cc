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
//<ID> 2123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    void *mem = NULL;
    char *mstr = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *true_item = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    cJSON_bool is_true = 0;
    double value0 = 0.0;
    int iv = 0;
    int validation = 0;

    // step 2: Setup
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(42.0);
    true_item = cJSON_CreateTrue();
    num2 = cJSON_CreateNumber(3.14);
    mem = cJSON_malloc(16);
    mstr = (char *)mem;

    // step 3: Core operations
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, true_item);
    cJSON_AddItemToArray(arr, num2);
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    is_true = cJSON_IsTrue(got1);
    value0 = cJSON_GetNumberValue(got0);
    iv = (int)value0;
    mstr[0] = (char)('0' + (iv % 10));
    mstr[1] = '\0';

    // step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(value0 == 42.0);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(mstr[0] != '\0');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(mem);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}