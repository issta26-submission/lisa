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
//<ID> 2164
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
    cJSON *name_item = NULL;
    cJSON *active_item = NULL;
    cJSON *got_name = NULL;
    cJSON *got_active = NULL;
    char *printed = NULL;
    const char *got_name_str = NULL;
    cJSON_bool is_active = 0;
    int validation = 0;

    // step 2: Setup (initialize object and add members)
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("Device42");
    cJSON_AddItemToObject(root, "name", name_item);
    active_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "active", active_item);

    // step 3: Operate (serialize and retrieve members)
    printed = cJSON_PrintUnformatted(root);
    got_name = cJSON_GetObjectItem(root, "name");
    got_name_str = cJSON_GetStringValue(got_name);
    got_active = cJSON_GetObjectItem(root, "active");
    is_active = cJSON_IsTrue(got_active);

    // step 4: Validate (accumulate checks)
    validation ^= (int)(root != NULL);
    validation ^= (int)(printed != NULL);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(got_name_str != NULL);
    validation ^= (int)(got_active != NULL);
    validation ^= (int)(is_active != 0);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}