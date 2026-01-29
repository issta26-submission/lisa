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
//<ID> 1938
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
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *rep_num = NULL;
    cJSON *got_item = NULL;
    cJSON *enabled = NULL;
    char *out = NULL;
    double set_ret = 0.0;
    double got_val = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", arr);
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);

    // step 3: Core operations
    rep_num = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemInArray(arr, 1, rep_num);
    set_ret = cJSON_SetNumberHelper(rep_num, 100.5);
    enabled = cJSON_AddTrueToObject(root, "enabled");
    out = cJSON_PrintUnformatted(root);
    got_item = cJSON_GetArrayItem(arr, 1);
    got_val = cJSON_GetNumberValue(got_item);
    is_true = cJSON_IsTrue(enabled);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(rep_num != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(set_ret == 100.5);
    validation ^= (int)(got_val == 100.5);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}