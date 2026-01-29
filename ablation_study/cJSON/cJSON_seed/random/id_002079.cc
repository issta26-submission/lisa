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
//<ID> 2079
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
    cJSON *array = NULL;
    cJSON *true_item1 = NULL;
    cJSON *true_item2 = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    int *size_ptr = NULL;
    int arr_size = 0;
    cJSON_bool is_true_bool = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    true_item1 = cJSON_CreateTrue();
    true_item2 = cJSON_CreateTrue();
    size_ptr = (int *)cJSON_malloc(sizeof(int));
    memset(size_ptr, 0, sizeof(int));

    // step 3: Configure
    cJSON_AddItemToArray(array, true_item1);
    cJSON_AddItemToArray(array, true_item2);
    arr_size = cJSON_GetArraySize(array);
    *size_ptr = arr_size;

    // step 4: Operate
    got0 = cJSON_GetArrayItem(array, 0);
    got1 = cJSON_GetArrayItem(array, 1);
    is_true_bool = cJSON_IsTrue(got0);

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(true_item1 != NULL);
    validation ^= (int)(true_item2 != NULL);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(is_true_bool != 0);
    validation ^= (int)(*size_ptr == 2);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(size_ptr);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}