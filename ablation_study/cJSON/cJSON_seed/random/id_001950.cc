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
//<ID> 1950
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
    cJSON *message = NULL;
    cJSON *got_msg = NULL;
    cJSON *got_flag = NULL;
    char *msg_str = NULL;
    cJSON_bool added_flag = 0;
    cJSON_bool added_msg = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    flag = cJSON_CreateFalse();
    message = cJSON_CreateString("ready");
    added_flag = cJSON_AddItemToObject(root, "ok", flag);
    added_msg = cJSON_AddItemToObject(root, "status", message);

    // step 3: Core operations
    got_msg = cJSON_GetObjectItem(root, "status");
    msg_str = cJSON_GetStringValue(got_msg);
    got_flag = cJSON_GetObjectItem(root, "ok");

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(flag != NULL);
    validation ^= (int)(message != NULL);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(added_msg != 0);
    validation ^= (int)(got_msg != NULL);
    validation ^= (int)(msg_str != NULL);
    validation ^= (int)(got_flag != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}