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
//<ID> 2172
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
    cJSON *explicit_item = NULL;
    cJSON *got_true = NULL;
    cJSON *got_false = NULL;
    char *printed = NULL;
    cJSON_bool added_true = 0;
    cJSON_bool added_false = 0;
    cJSON_bool is_array = 0;
    int validation = 0;

    // step 2: Setup (Initialize object and add booleans)
    root = cJSON_CreateObject();
    true_item = cJSON_CreateTrue();
    false_item = cJSON_CreateFalse();
    added_true = cJSON_AddItemToObject(root, "trueItem", true_item);
    added_false = cJSON_AddItemToObject(root, "falseItem", false_item);
    explicit_item = cJSON_AddBoolToObject(root, "explicitBool", (cJSON_bool)1);

    // step 3: Operate (serialize and retrieve values)
    printed = cJSON_PrintUnformatted(root);
    got_true = cJSON_GetObjectItem(root, "trueItem");
    got_false = cJSON_GetObjectItem(root, "falseItem");
    is_array = cJSON_IsArray(root);

    // step 4: Validate (accumulate simple checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(explicit_item != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_true != NULL);
    validation ^= (int)(got_false != NULL);
    validation ^= (int)(added_true != 0);
    validation ^= (int)(added_false != 0);
    validation ^= (int)(is_array == 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}