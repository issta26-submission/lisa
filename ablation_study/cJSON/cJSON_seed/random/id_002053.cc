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
//<ID> 2053
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
    cJSON *implicit_true = NULL;
    cJSON *implicit_false = NULL;
    cJSON_bool explicit_added_true = 0;
    cJSON_bool explicit_added_false = 0;
    cJSON *got_implicit_true = NULL;
    cJSON *got_explicit_true = NULL;
    cJSON *got_implicit_false = NULL;
    cJSON_bool is_bool_implicit_true = 0;
    cJSON_bool is_bool_explicit_true = 0;
    cJSON_bool is_bool_implicit_false = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    explicit_added_true = cJSON_AddItemToObject(root, "explicit_true", true_item);
    explicit_added_false = cJSON_AddItemToObject(root, "explicit_false", false_item);
    implicit_true = cJSON_AddBoolToObject(root, "implicit_true", 1);
    implicit_false = cJSON_AddBoolToObject(root, "implicit_false", 0);

    // step 4: Operate
    got_implicit_true = cJSON_GetObjectItemCaseSensitive(root, "implicit_true");
    got_explicit_true = cJSON_GetObjectItemCaseSensitive(root, "explicit_true");
    got_implicit_false = cJSON_GetObjectItemCaseSensitive(root, "implicit_false");
    is_bool_implicit_true = cJSON_IsBool(got_implicit_true);
    is_bool_explicit_true = cJSON_IsBool(got_explicit_true);
    is_bool_implicit_false = cJSON_IsBool(got_implicit_false);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(implicit_true != NULL);
    validation ^= (int)(implicit_false != NULL);
    validation ^= (int)(explicit_added_true != 0);
    validation ^= (int)(explicit_added_false != 0);
    validation ^= (int)(got_implicit_true != NULL);
    validation ^= (int)(got_explicit_true != NULL);
    validation ^= (int)(got_implicit_false != NULL);
    validation ^= (int)(is_bool_implicit_true != 0);
    validation ^= (int)(is_bool_explicit_true != 0);
    validation ^= (int)(is_bool_implicit_false != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}