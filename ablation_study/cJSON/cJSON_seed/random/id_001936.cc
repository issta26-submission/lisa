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
//<ID> 1936
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
    cJSON *item0 = NULL;
    cJSON *item1 = NULL;
    cJSON *replacement = NULL;
    char *out = NULL;
    double set_ret = 0.0;
    double got_val = 0.0;
    cJSON_bool flag_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    item0 = cJSON_CreateNumber(1.0);
    item1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, item0);
    cJSON_AddItemToArray(arr, item1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddTrueToObject(root, "flag");
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    replacement = cJSON_CreateNumber(42.0);
    cJSON_ReplaceItemInArray(arr, 0, replacement);
    set_ret = cJSON_SetNumberHelper(replacement, 84.0);
    got_val = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(item0 != NULL);
    validation ^= (int)(item1 != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(set_ret == 84.0);
    validation ^= (int)(got_val == 84.0);
    validation ^= (int)(flag_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}