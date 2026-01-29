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
//<ID> 2075
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
    cJSON *array = NULL;
    cJSON *true_item = NULL;
    cJSON *num_item = NULL;
    cJSON *got_true = NULL;
    cJSON *got_num = NULL;
    cJSON_bool is_true_flag = 0;
    int count = 0;
    void *mem = NULL;
    int *storage = NULL;
    double num_value = 0.0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    num_item = cJSON_CreateNumber(7.0);

    // step 3: Configure
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, num_item);

    // step 4: Operate
    count = cJSON_GetArraySize(array);
    got_true = cJSON_GetArrayItem(array, 0);
    got_num = cJSON_GetArrayItem(array, 1);
    is_true_flag = cJSON_IsTrue(got_true);
    num_value = cJSON_GetNumberValue(got_num);
    mem = cJSON_malloc((size_t)count * sizeof(int));
    storage = (int *)mem;
    storage[0] = (int)is_true_flag;
    storage[1] = (int)num_value;

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(got_num != NULL);
    validation ^= (int)(count == 2);
    validation ^= (int)(storage[0] == 1);
    validation ^= (int)(storage[1] == 7);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(mem);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}