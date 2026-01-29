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
//<ID> 2078
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
    cJSON *true_item1 = NULL;
    cJSON *true_item2 = NULL;
    cJSON *got_item0 = NULL;
    cJSON *got_item1 = NULL;
    void *memblock = NULL;
    int arr_size = 0;
    cJSON_bool is_item0_true = 0;
    cJSON_bool is_item1_true = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    true_item1 = cJSON_CreateTrue();
    true_item2 = cJSON_CreateTrue();
    memblock = cJSON_malloc(sizeof(int) * 2);
    ((int *)memblock)[0] = 123;
    ((int *)memblock)[1] = 456;

    // step 3: Configure
    cJSON_AddItemToArray(array, true_item1);
    cJSON_AddItemToArray(array, true_item2);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(array);
    got_item0 = cJSON_GetArrayItem(array, 0);
    got_item1 = cJSON_GetArrayItem(array, 1);
    is_item0_true = cJSON_IsTrue(got_item0);
    is_item1_true = cJSON_IsTrue(got_item1);

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(true_item1 != NULL);
    validation ^= (int)(true_item2 != NULL);
    validation ^= (int)(memblock != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(got_item0 != NULL);
    validation ^= (int)(got_item1 != NULL);
    validation ^= (int)(is_item0_true != 0);
    validation ^= (int)(is_item1_true != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(memblock);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}