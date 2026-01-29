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
//<ID> 2103
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
    cJSON *true_item = NULL;
    cJSON *dup_item = NULL;
    char *unformatted = NULL;
    char *dup_strval = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_AddStringToObject(root, "message", "hello");
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "enabled", true_item);

    // step 3: Operate
    dup_item = cJSON_Duplicate((const cJSON *)str_item, 1);
    dup_strval = cJSON_GetStringValue((const cJSON *)dup_item);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(dup_strval != NULL);
    validation ^= (int)(unformatted != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(dup_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}