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
//<ID> 1952
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
    cJSON *flag = NULL;
    cJSON *name = NULL;
    cJSON *got_flag = NULL;
    cJSON *got_name = NULL;
    char *name_val = NULL;
    cJSON_bool added_flag = 0;
    cJSON_bool added_name = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    flag = cJSON_CreateFalse();
    name = cJSON_CreateString("example");
    added_flag = cJSON_AddItemToObject(root, "flag", flag);
    added_name = cJSON_AddItemToObject(root, "name", name);

    // step 3: Core operations
    got_flag = cJSON_GetObjectItem(root, "flag");
    got_name = cJSON_GetObjectItem(root, "name");
    name_val = cJSON_GetStringValue(got_name);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(flag != NULL);
    validation ^= (int)(name != NULL);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(added_name != 0);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(got_name != NULL);
    validation ^= (int)(name_val != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}