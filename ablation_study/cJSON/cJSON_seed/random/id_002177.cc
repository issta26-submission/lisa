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
//<ID> 2177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[32];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *bool_added = NULL;
    cJSON *got_true = NULL;
    char *printed = NULL;
    cJSON_bool root_is_array = 0;
    cJSON_bool got_true_is_array = 0;
    cJSON_bool added_bool_is_array = 0;
    int validation = 0;

    // step 2: Setup (create object and boolean items)
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "explicit_true", true_item);
    cJSON_AddItemToObject(root, "explicit_false", false_item);
    bool_added = cJSON_AddBoolToObject(root, "added_bool_true", 1);

    // step 3: Operate (serialize and query)
    printed = cJSON_PrintUnformatted(root);
    got_true = cJSON_GetObjectItem(root, "explicit_true");
    root_is_array = cJSON_IsArray(root);
    got_true_is_array = cJSON_IsArray(got_true);
    added_bool_is_array = cJSON_IsArray(bool_added);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(bool_added != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(root_is_array == 0);
    validation ^= (int)(got_true_is_array == 0);
    validation ^= (int)(added_bool_is_array == 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}