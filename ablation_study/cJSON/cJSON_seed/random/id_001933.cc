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
//<ID> 1933
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
    cJSON *n1 = NULL;
    cJSON *n2 = NULL;
    cJSON *newitem = NULL;
    cJSON *got_item = NULL;
    cJSON *got_active = NULL;
    char *out1 = NULL;
    char *out2 = NULL;
    double set_ret = 0.0;
    double got_val = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool active_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_AddTrueToObject(root, "active");
    out1 = cJSON_PrintUnformatted(root);

    // step 3: Core operations
    newitem = cJSON_CreateNumber(42.0);
    replaced = cJSON_ReplaceItemInArray(arr, 1, newitem);
    set_ret = cJSON_SetNumberHelper(newitem, 7.5);
    got_item = cJSON_GetArrayItem(arr, 1);
    got_val = cJSON_GetNumberValue(got_item);
    got_active = cJSON_GetObjectItem(root, "active");
    active_flag = cJSON_IsTrue(got_active);
    out2 = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(n1 != NULL);
    validation ^= (int)(n2 != NULL);
    validation ^= (int)(newitem != NULL);
    validation ^= (int)(out1 != NULL);
    validation ^= (int)(out2 != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(set_ret == 7.5);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_val == 7.5);
    validation ^= (int)(active_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out1);
    cJSON_free(out2);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}