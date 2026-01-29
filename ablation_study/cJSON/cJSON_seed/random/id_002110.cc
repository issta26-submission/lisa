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
//<ID> 2110
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
    cJSON *num_item = NULL;
    cJSON *got = NULL;
    cJSON *replacement = NULL;
    cJSON *got_after = NULL;
    cJSON *true_item = NULL;
    cJSON_bool flag_true = 0;
    double val_before = 0.0;
    double val_after = 0.0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", num_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate
    got = cJSON_GetObjectItemCaseSensitive(root, "answer");
    val_before = cJSON_GetNumberValue(got);
    replacement = cJSON_CreateNumber(43.0);
    cJSON_ReplaceItemViaPointer(root, got, replacement);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(root, "flag"));
    got_after = cJSON_GetObjectItemCaseSensitive(root, "answer");
    val_after = cJSON_GetNumberValue(got_after);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(got != NULL);
    validation ^= (int)(got_after != NULL);
    validation ^= (int)(val_before == 42.0);
    validation ^= (int)(val_after == 43.0);
    validation ^= (int)(flag_true != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}