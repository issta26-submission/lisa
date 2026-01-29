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
//<ID> 2071
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *got0 = NULL;
    cJSON *got1 = NULL;
    int size = 0;
    void *mem = NULL;
    cJSON_bool is0 = 0;
    cJSON_bool is1 = 0;
    int validation = 0;

    // step 2: Setup
    array = cJSON_CreateArray();
    item0 = cJSON_CreateTrue();
    item1 = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(array, item0);
    cJSON_AddItemToArray(array, item1);

    // step 4: Operate
    size = cJSON_GetArraySize(array);
    mem = cJSON_malloc((size_t)(size + 1));
    got0 = cJSON_GetArrayItem(array, 0);
    got1 = cJSON_GetArrayItem(array, 1);
    is0 = cJSON_IsTrue(got0);
    is1 = cJSON_IsTrue(got1);

    // step 5: Validate
    validation ^= (int)(array != NULL);
    validation ^= (int)(item0 != NULL);
    validation ^= (int)(item1 != NULL);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(size == 2);
    validation ^= (int)(got0 != NULL);
    validation ^= (int)(got1 != NULL);
    validation ^= (int)(is0 != 0);
    validation ^= (int)(is1 != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(mem);
    cJSON_Delete(array);

    // API sequence test completed successfully
    return 66;
}