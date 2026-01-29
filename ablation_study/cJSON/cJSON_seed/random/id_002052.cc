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
//<ID> 2052
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *true_item = NULL;
    cJSON *false_item = NULL;
    cJSON *added_bool_item = NULL;
    cJSON *got_true = NULL;
    cJSON *got_false = NULL;
    cJSON_bool added_true = 0;
    cJSON_bool added_false = 0;
    cJSON_bool check_true = 0;
    cJSON_bool check_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_true = cJSON_AddItemToObject(root, "true_custom", true_item);
    added_false = cJSON_AddItemToObject(root, "false_custom", false_item);
    added_bool_item = cJSON_AddBoolToObject(root, "added_flag", 1);

    // step 4: Operate / Inspect
    got_true = cJSON_GetObjectItemCaseSensitive(root, "true_custom");
    got_false = cJSON_GetObjectItemCaseSensitive(root, "false_custom");
    check_true = cJSON_IsBool(got_true);
    check_false = cJSON_IsBool(got_false);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_bool_item != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(got_false != NULL);
    validation ^= (int)(added_true != 0);
    validation ^= (int)(added_false != 0);
    validation ^= (int)(check_true != 0);
    validation ^= (int)(check_false != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}