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
//<ID> 2073
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char *buf = (char *)cJSON_malloc(32);
    memset(buf, 0, 32);
    cJSON *array = NULL;
    cJSON *true_item_a = NULL;
    cJSON *true_item_b = NULL;
    cJSON *got_item_0 = NULL;
    cJSON *got_item_1 = NULL;
    int arr_size = 0;
    cJSON_bool is0 = 0;
    cJSON_bool is1 = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    true_item_a = cJSON_CreateTrue();
    true_item_b = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(array, true_item_a);
    cJSON_AddItemToArray(array, true_item_b);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(array);
    got_item_0 = cJSON_GetArrayItem(array, 0);
    got_item_1 = cJSON_GetArrayItem(array, 1);
    is0 = cJSON_IsTrue(got_item_0);
    is1 = cJSON_IsTrue(got_item_1);

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(true_item_a != NULL);
    validation ^= (int)(true_item_b != NULL);
    validation ^= (int)(got_item_0 != NULL);
    validation ^= (int)(got_item_1 != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(is0 != 0);
    validation ^= (int)(is1 != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(buf);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}