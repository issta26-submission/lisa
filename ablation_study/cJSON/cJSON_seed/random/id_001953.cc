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
//<ID> 1953
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
    cJSON *bool_item = NULL;
    cJSON *got_str = NULL;
    cJSON_bool added_str = 0;
    cJSON_bool added_bool = 0;
    char *strval = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    bool_item = cJSON_CreateFalse();
    added_str = cJSON_AddItemToObject(root, "greeting", str_item);
    added_bool = cJSON_AddItemToObject(root, "enabled", bool_item);

    // step 3: Core operations
    got_str = cJSON_GetObjectItem(root, "greeting");
    strval = cJSON_GetStringValue(got_str);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(added_str != 0);
    validation ^= (int)(added_bool != 0);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(strval != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}