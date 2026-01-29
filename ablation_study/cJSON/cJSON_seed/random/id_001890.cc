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
//<ID> 1890
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
    cJSON *null_in_array = NULL;
    cJSON *null_in_object = NULL;
    cJSON *true_item = NULL;
    cJSON *got_true = NULL;
    cJSON_bool added_arr = 0;
    cJSON_bool added_obj_list = 0;
    cJSON_bool added_obj_alone = 0;
    cJSON_bool is_true = 0;
    char *out = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    added_obj_list = cJSON_AddItemToObject(root, "list", arr);
    null_in_array = cJSON_CreateNull();
    added_arr = cJSON_AddItemToArray(arr, null_in_array);
    null_in_object = cJSON_CreateNull();
    added_obj_alone = cJSON_AddItemToObject(root, "empty", null_in_object);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Core operations
    got_true = cJSON_GetObjectItem(root, "flag");
    is_true = cJSON_IsTrue(got_true);
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(null_in_array != NULL);
    validation ^= (int)(null_in_object != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(added_obj_list != 0);
    validation ^= (int)(added_arr != 0);
    validation ^= (int)(added_obj_alone != 0);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}