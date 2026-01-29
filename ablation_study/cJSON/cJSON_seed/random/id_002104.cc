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
//<ID> 2104
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
    cJSON *dup_item = NULL;
    cJSON *true_item = NULL;
    char *unformatted = NULL;
    const char *dup_str = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_AddStringToObject(root, "greeting", "hello");
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Core operations
    dup_item = cJSON_Duplicate(str_item, 1);
    dup_str = cJSON_GetStringValue(dup_item);
    unformatted = cJSON_PrintUnformatted(root);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(dup_item != NULL);
    validation ^= (int)(dup_str != NULL);
    validation ^= (int)(unformatted != NULL);
    validation ^= (int)(dup_str[0] == 'h');
    validation ^= (int)(dup_str[1] == 'e');
    validation ^= (int)(dup_str[2] == 'l');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(unformatted);
    cJSON_Delete(dup_item);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}