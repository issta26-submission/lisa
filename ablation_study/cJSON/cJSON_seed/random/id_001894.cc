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
//<ID> 1894
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
    cJSON *got_item = NULL;
    char *out = NULL;
    cJSON_bool is_true = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);
    null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", null_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Core operations
    str_item = cJSON_CreateString("element");
    cJSON_AddItemToArray(arr, str_item);
    got_item = cJSON_GetArrayItem(arr, 0);
    is_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "enabled"));
    out = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(arr != NULL);
    validation ^= (int)(null_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_item == str_item);
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