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
//<ID> 2105
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
    cJSON *name_item = NULL;
    cJSON *true_item = NULL;
    cJSON *dup_root = NULL;
    cJSON *dup_name = NULL;
    char *name_value = NULL;
    char *unformatted = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    name_item = cJSON_AddStringToObject(root, "name", "Alice");
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", true_item);

    // step 3: Operate
    dup_root = cJSON_Duplicate(root, 1);
    dup_name = cJSON_GetObjectItem(dup_root, "name");
    name_value = cJSON_GetStringValue(dup_name);
    unformatted = cJSON_PrintUnformatted(dup_root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(name_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(dup_root != NULL);
    validation ^= (int)(dup_name != NULL);
    validation ^= (int)(name_value != NULL);
    validation ^= (int)(unformatted != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(dup_root);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}