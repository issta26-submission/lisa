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
//<ID> 2042
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
    cJSON *str_item = NULL;
    cJSON *true_item = NULL;
    cJSON *bool_added = NULL;
    cJSON_bool added_true = 0;
    void *mem = NULL;
    char *sval = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello_world");
    true_item = cJSON_CreateTrue();
    mem = cJSON_malloc((size_t)32);
    memset(mem, 0, 32);

    // step 3: Configure
    bool_added = cJSON_AddBoolToObject(root, "enabled", (cJSON_bool)1);
    added_true = cJSON_AddItemToObject(root, "explicit_true", true_item);
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate
    sval = cJSON_GetStringValue(str_item);
    *((char *)mem) = sval[0];
    cJSON_IsInvalid(str_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(bool_added != NULL);
    validation ^= (int)(sval != NULL);
    validation ^= (int)(mem != NULL);
    validation ^= (int)(added_true != 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}