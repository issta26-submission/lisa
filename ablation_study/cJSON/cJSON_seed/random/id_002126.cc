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
//<ID> 2126
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    size_t buf_len = 64;
    char *buf = (char *)cJSON_malloc(buf_len);
    memset(buf, 0, buf_len);
    cJSON *array = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *got_num1 = NULL;
    cJSON *got_num2 = NULL;
    cJSON *got_true_item = NULL;
    cJSON *true_item = NULL;
    double value1 = 0.0;
    double value2 = 0.0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.5);
    num2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(array, true_item);

    // step 3: Operate
    got_num1 = cJSON_GetArrayItem(array, 0);
    got_num2 = cJSON_GetArrayItem(array, 1);
    got_true_item = cJSON_GetArrayItem(array, 2);
    value1 = cJSON_GetNumberValue(got_num1);
    value2 = cJSON_GetNumberValue(got_num2);
    is_true = cJSON_IsTrue(got_true_item);

    // step 4: Validate
    validation ^= (int)(buf != NULL);
    validation ^= (int)(array != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(got_num1 != NULL);
    validation ^= (int)(got_num2 != NULL);
    validation ^= (int)(got_true_item != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(value1 == 1.5);
    validation ^= (int)(value2 == 2.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(buf);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}