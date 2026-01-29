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
//<ID> 1951
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
    cJSON *false_item = NULL;
    cJSON *str_item = NULL;
    cJSON *got_false = NULL;
    cJSON *got_str = NULL;
    char *got_strval = NULL;
    cJSON_bool added_flag1 = 0;
    cJSON_bool added_flag2 = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    false_item = cJSON_CreateFalse();
    str_item = cJSON_CreateString("hello");
    added_flag1 = cJSON_AddItemToObject(root, "flag", false_item);
    added_flag2 = cJSON_AddItemToObject(root, "msg", str_item);

    // step 3: Core operations
    got_false = cJSON_GetObjectItem(root, "flag");
    got_str = cJSON_GetObjectItem(root, "msg");
    got_strval = cJSON_GetStringValue(got_str);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(added_flag1 != 0);
    validation ^= (int)(added_flag2 != 0);
    validation ^= (int)(got_false != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(got_strval != NULL);
    validation ^= (int)(got_strval[0] == 'h');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}