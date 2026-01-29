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
//<ID> 2076
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));
    cJSON *array = NULL;
    cJSON *t1 = NULL;
    cJSON *t2 = NULL;
    cJSON *got = NULL;
    cJSON_bool got_true = 0;
    char *allocbuf = NULL;
    int arr_size = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    t1 = cJSON_CreateTrue();
    t2 = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(array, t1);
    cJSON_AddItemToArray(array, t2);

    // step 4: Operate
    arr_size = cJSON_GetArraySize(array);
    got = cJSON_GetArrayItem(array, 1);
    got_true = cJSON_IsTrue(got);
    allocbuf = (char *)cJSON_malloc(16);
    memset(allocbuf, 0, 16);
    allocbuf[0] = (char)(arr_size + '0');

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(t1 != NULL);
    validation ^= (int)(t2 != NULL);
    validation ^= (int)(got != NULL);
    validation ^= (int)(got_true != 0);
    validation ^= (int)(allocbuf != NULL);
    scratch[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(allocbuf);
    cJSON_Delete(array);
    // API sequence test completed successfully
    return 66;
}