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
//<ID> 2057
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
    cJSON *added_auto_true = NULL;
    cJSON *added_auto_false = NULL;
    cJSON *got_auto_true = NULL;
    cJSON *got_manual_false = NULL;
    cJSON_bool add_manual_true = 0;
    cJSON_bool add_manual_false = 0;
    cJSON_bool is_bool_auto_true = 0;
    cJSON_bool is_bool_manual_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    added_auto_true = cJSON_AddBoolToObject(root, "auto_true", 1);
    added_auto_false = cJSON_AddBoolToObject(root, "auto_false", 0);
    add_manual_true = cJSON_AddItemToObject(root, "manual_true", true_item);
    add_manual_false = cJSON_AddItemToObject(root, "manual_false", false_item);

    // step 4: Operate / Inspect
    got_auto_true = cJSON_GetObjectItemCaseSensitive(root, "auto_true");
    got_manual_false = cJSON_GetObjectItemCaseSensitive(root, "manual_false");
    is_bool_auto_true = cJSON_IsBool(got_auto_true);
    is_bool_manual_false = cJSON_IsBool(got_manual_false);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_auto_true != NULL);
    validation ^= (int)(added_auto_false != NULL);
    validation ^= (int)(add_manual_true != 0);
    validation ^= (int)(add_manual_false != 0);
    validation ^= (int)(got_auto_true != NULL);
    validation ^= (int)(got_manual_false != NULL);
    validation ^= (int)(is_bool_auto_true != 0);
    validation ^= (int)(is_bool_manual_false != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    return 66;
    // API sequence test completed successfully
}