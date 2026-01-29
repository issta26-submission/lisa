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
//<ID> 2072
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
    cJSON *got_item = NULL;
    int size = 0;
    int *size_copy = NULL;
    cJSON_bool add_ok = 0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    add_ok = cJSON_AddItemToArray(array, true_item);

    // step 4: Operate
    size = cJSON_GetArraySize(array);
    got_item = cJSON_GetArrayItem(array, 0);
    is_true = cJSON_IsTrue(got_item);
    size_copy = (int *)cJSON_malloc(sizeof(int));
    *size_copy = size;

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(add_ok != 0);
    validation ^= (int)(size == 1);
    validation ^= (int)(got_item == true_item);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(size_copy != NULL);
    validation ^= (int)(*size_copy == size);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(size_copy);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}