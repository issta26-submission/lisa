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
//<ID> 2055
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
    cJSON *created_true = NULL;
    cJSON *created_false = NULL;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON *added_via_addbool_true = NULL;
    cJSON *added_via_addbool_false = NULL;
    cJSON *got_manual_true = NULL;
    cJSON *got_manual_false = NULL;
    cJSON *got_auto_true = NULL;
    cJSON *got_auto_false = NULL;
    cJSON_bool is_bool1 = 0;
    cJSON_bool is_bool2 = 0;
    cJSON_bool is_bool3 = 0;
    cJSON_bool is_bool4 = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    created_true = cJSON_CreateTrue();
    created_false = cJSON_CreateFalse();

    // step 3: Configure
    add_ok1 = cJSON_AddItemToObject(root, "manual_true", created_true);
    add_ok2 = cJSON_AddItemToObject(root, "manual_false", created_false);
    added_via_addbool_true = cJSON_AddBoolToObject(root, "auto_true", 1);
    added_via_addbool_false = cJSON_AddBoolToObject(root, "auto_false", 0);

    // step 4: Operate
    got_manual_true = cJSON_GetObjectItemCaseSensitive(root, "manual_true");
    got_manual_false = cJSON_GetObjectItemCaseSensitive(root, "manual_false");
    got_auto_true = cJSON_GetObjectItemCaseSensitive(root, "auto_true");
    got_auto_false = cJSON_GetObjectItemCaseSensitive(root, "auto_false");
    is_bool1 = cJSON_IsBool(got_manual_true);
    is_bool2 = cJSON_IsBool(got_manual_false);
    is_bool3 = cJSON_IsBool(got_auto_true);
    is_bool4 = cJSON_IsBool(got_auto_false);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(created_true != NULL);
    validation ^= (int)(created_false != NULL);
    validation ^= (int)(add_ok1 != 0);
    validation ^= (int)(add_ok2 != 0);
    validation ^= (int)(added_via_addbool_true != NULL);
    validation ^= (int)(added_via_addbool_false != NULL);
    validation ^= (int)(got_manual_true != NULL);
    validation ^= (int)(got_manual_false != NULL);
    validation ^= (int)(got_auto_true != NULL);
    validation ^= (int)(got_auto_false != NULL);
    validation ^= (int)(is_bool1 != 0);
    validation ^= (int)(is_bool2 != 0);
    validation ^= (int)(is_bool3 != 0);
    validation ^= (int)(is_bool4 != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}