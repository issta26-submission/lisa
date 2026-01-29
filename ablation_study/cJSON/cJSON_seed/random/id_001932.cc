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
//<ID> 1932
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
    cJSON *replacement = NULL;
    cJSON *got_item = NULL;
    char *out = NULL;
    double set_ret = 0.0;
    double got_num = 0.0;
    cJSON_bool replaced_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num1 = cJSON_CreateNumber(1.0);
    num2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "nums", arr);
    cJSON_AddTrueToObject(root, "enabled");
    out = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    replacement = cJSON_CreateNumber(42.0);
    replaced_flag = cJSON_ReplaceItemInArray(arr, 0, replacement);
    got_item = cJSON_GetArrayItem(arr, 0);
    set_ret = cJSON_SetNumberHelper(got_item, 3.14);
    got_num = cJSON_GetNumberValue(got_item);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(num1 != NULL);
    validation ^= (int)(num2 != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(out != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(replaced_flag != 0);
    validation ^= (int)(set_ret == 3.14);
    validation ^= (int)(got_num == 3.14);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}