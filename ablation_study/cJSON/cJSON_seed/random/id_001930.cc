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
//<ID> 1930
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[64];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *n0 = NULL;
    cJSON *n1 = NULL;
    cJSON *repl = NULL;
    char *out = NULL;
    double set_ret = 0.0;
    double val0 = 0.0;
    double val1 = 0.0;
    cJSON_bool enabled_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n0 = cJSON_CreateNumber(1.0);
    n1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddTrueToObject(root, "enabled");

    // step 3: Core operations
    set_ret = cJSON_SetNumberHelper(n0, 3.14);
    repl = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 1, repl);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    val0 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    val1 = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 1));
    enabled_flag = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled"));
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n0 != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(repl != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(set_ret == 3.14);
    validation ^= (int)(val0 == 3.14);
    validation ^= (int)(val1 == 42.0);
    validation ^= (int)(enabled_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}