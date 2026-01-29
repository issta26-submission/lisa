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
//<ID> 2116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[8];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *num_item = NULL;
    cJSON *got_item = NULL;
    cJSON *replacement = NULL;
    cJSON *after = NULL;
    cJSON *true_item = NULL;
    double after_val = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool is_true_flag = 0;
    int validation = 0;

    // step 2: Setup (Initialize root and add items)
    root = cJSON_CreateObject();
    num_item = cJSON_AddNumberToObject(root, "value", 42.0);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "ok", true_item);

    // step 3: Core operations (retrieve, replace via pointer, re-retrieve, test true)
    got_item = cJSON_GetObjectItemCaseSensitive(root, "value");
    replacement = cJSON_CreateNumber(84.0);
    replaced = cJSON_ReplaceItemViaPointer(root, got_item, replacement);
    after = cJSON_GetObjectItemCaseSensitive(root, "value");
    after_val = cJSON_GetNumberValue(after);
    is_true_flag = cJSON_IsTrue(cJSON_GetObjectItemCaseSensitive(root, "ok"));

    // step 4: Validate (combine checks without branches)
    validation ^= (int)(root != NULL);
    validation ^= (int)(num_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(replacement != NULL);
    validation ^= (int)(replaced != 0);
    validation ^= (int)(after != NULL);
    validation ^= (int)(after_val == 84.0);
    validation ^= (int)(is_true_flag != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}