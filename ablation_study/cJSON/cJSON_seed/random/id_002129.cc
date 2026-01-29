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
//<ID> 2129
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
    void *mem = NULL;
    cJSON *arr = NULL;
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *t = NULL;
    cJSON_bool is_true = 0;
    double v0 = 0.0;
    double v1 = 0.0;
    int validation = 0;

    // step 2: Setup
    mem = cJSON_malloc(32);
    memset(mem, 0, 32);
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(3.14);
    n2 = cJSON_CreateNumber(2.718);
    t = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, t);

    // step 3: Operate
    v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    is_true = cJSON_IsTrue(cJSON_GetArrayItem(arr, 2));
    ((char *)mem)[0] = (char)(is_true + '0');

    // step 4: Validate
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(t != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(v0 == 3.14);
    validation ^= (int)(v1 == 2.718);
    buf[0] = (char)(validation + '0');
    ((char *)mem)[1] = buf[0];

    // step 5: Cleanup
    cJSON_free(mem);
    cJSON_Delete(arr);

    // API sequence test completed successfully
    return 66;
}