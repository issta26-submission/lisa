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
//<ID> 2077
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
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *true1 = NULL;
    cJSON *true2 = NULL;
    cJSON *got_item = NULL;
    void *temp_mem = NULL;
    int arr_size = 0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "flags", arr);
    true1 = cJSON_CreateTrue();
    true2 = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, true1);
    cJSON_AddItemToArray(arr, true2);
    temp_mem = cJSON_malloc(16);
    ((char *)temp_mem)[0] = 0;

    // step 4: Operate
    arr_size = cJSON_GetArraySize(arr);
    got_item = cJSON_GetArrayItem(arr, 1);
    is_true = cJSON_IsTrue(got_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(true1 != NULL);
    validation ^= (int)(true2 != NULL);
    validation ^= (int)(temp_mem != NULL);
    validation ^= (int)(arr_size == 2);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(is_true != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(temp_mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}