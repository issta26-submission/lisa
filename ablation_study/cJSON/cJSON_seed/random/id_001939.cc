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
//<ID> 1939
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
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *replacement = NULL;
    cJSON *got_item0 = NULL;
    cJSON *got_active = NULL;
    char *out = NULL;
    double helper_ret = 0.0;
    double got_val = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool active_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(1.0);
    num1 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddTrueToObject(root, "active");

    // step 3: Core operations
    replacement = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemInArray(arr, 0, replacement);
    helper_ret = cJSON_SetNumberHelper(replacement, 84.0);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    got_item0 = cJSON_GetArrayItem(arr, 0);
    got_val = cJSON_GetNumberValue(got_item0);
    got_active = cJSON_GetObjectItem(root, "active");
    active_flag = cJSON_IsTrue(got_active);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num0 != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(helper_ret == 84.0);
    validation ^= (int)(got_item0 != NULL);
    validation ^= (int)(got_val == 84.0);
    validation ^= (int)(active_flag != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}