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
//<ID> 2040
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
    cJSON *str_item = NULL;
    cJSON *got_item = NULL;
    cJSON *true_item = NULL;
    cJSON *added_bool_item = NULL;
    char *alloc_buf = NULL;
    char *got_str = NULL;
    cJSON_bool invalid_check = 0;
    int validation = 0;

    // step 2: Setup / Allocate and create items
    root = cJSON_CreateObject();
    alloc_buf = (char *)cJSON_malloc(16);
    memset(alloc_buf, 0, 16);
    memcpy(alloc_buf, "hello", 6);
    str_item = cJSON_CreateString(alloc_buf);
    cJSON_AddItemToObject(root, "greeting", str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "explicit_true", true_item);
    added_bool_item = cJSON_AddBoolToObject(root, "implicit_true", (cJSON_bool)1);

    // step 3: Operate / Inspect values
    got_item = cJSON_GetObjectItem(root, "greeting");
    got_str = cJSON_GetStringValue(got_item);
    invalid_check = cJSON_IsInvalid(got_item);

    // step 4: Validate internal state (no branches)
    validation ^= (int)(root != NULL);
    validation ^= (int)(alloc_buf != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(got_item != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(invalid_check == 0);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(added_bool_item != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup cJSON structures
    cJSON_Delete(root);

    // step 6: Cleanup allocated memory
    cJSON_free(alloc_buf);

    // API sequence test completed successfully
    return 66;
}