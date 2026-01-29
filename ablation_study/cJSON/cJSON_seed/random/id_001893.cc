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
//<ID> 1893
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
    cJSON *str_item = NULL;
    cJSON *got_from_arr = NULL;
    cJSON *got_null = NULL;
    char *out = NULL;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "array", arr);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nothing", null_item);

    // step 3: Core operations
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);
    str_item = cJSON_CreateString("value");
    cJSON_AddItemToArray(arr, str_item);

    // step 4: Validate
    got_from_arr = cJSON_GetArrayItem(arr, 0);
    is_true = cJSON_IsTrue(got_from_arr);
    got_null = cJSON_GetObjectItem(root, "nothing");
    out = cJSON_PrintUnformatted(root);
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(null_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got_from_arr == true_item);
    validation ^= (int)(is_true != 0);
    validation ^= (int)(got_null != NULL);
    validation ^= (int)(out != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}