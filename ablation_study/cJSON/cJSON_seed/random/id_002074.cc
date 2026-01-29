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
//<ID> 2074
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
    cJSON *false_item = NULL;
    cJSON *got_item = NULL;
    void *mem = NULL;
    int arr_size = 0;
    cJSON_bool got_true = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    mem = cJSON_malloc(16);
    memset(mem, 0, 16);

    // step 3: Configure
    cJSON_AddItemToArray(array, true_item);
    cJSON_AddItemToArray(array, false_item);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(array);
    got_item = cJSON_GetArrayItem(array, 0);
    got_true = cJSON_IsTrue(got_item);

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_true != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(mem);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}