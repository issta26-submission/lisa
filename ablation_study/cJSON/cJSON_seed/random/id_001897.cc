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
//<ID> 1897
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
    cJSON *null_item = NULL;
    cJSON *true_item = NULL;
    cJSON *got_true = NULL;
    char *out = NULL;
    cJSON_bool add_arr_result = 0;
    cJSON_bool add_items_result = 0;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    add_arr_result = cJSON_AddItemToObject(root, "items", arr);
    null_item = cJSON_CreateNull();
    add_items_result = cJSON_AddItemToArray(arr, null_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Core operations
    got_true = cJSON_GetObjectItem(root, "enabled");
    is_true = cJSON_IsTrue(got_true);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(null_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(out != NULL);
    validation ^= (int)(add_arr_result != 0);
    validation ^= (int)(add_items_result != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}