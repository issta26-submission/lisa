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
//<ID> 2049
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
    cJSON *bool_item = NULL;
    char *allocated = NULL;
    const char *got_str = NULL;
    cJSON_bool invalid_flag = 0;
    int validation = 0;

    // step 2: Setup
    allocated = (char*)cJSON_malloc(6);
    allocated[0] = 't';
    allocated[1] = 'e';
    allocated[2] = 's';
    allocated[3] = 't';
    allocated[4] = '!';
    allocated[5] = '\0';
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString(allocated);

    // step 3: Configure
    cJSON_AddItemToObject(root, "greeting", str_item);
    true_item = cJSON_CreateTrue();
    bool_item = cJSON_AddBoolToObject(root, "flag", 1);

    // step 4: Operate
    got_str = cJSON_GetStringValue(str_item);
    invalid_flag = cJSON_IsInvalid(bool_item);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(bool_item != NULL);
    validation ^= (int)(allocated != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(got_str[0] == 't');
    validation ^= (int)(invalid_flag == 0);
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(allocated);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}