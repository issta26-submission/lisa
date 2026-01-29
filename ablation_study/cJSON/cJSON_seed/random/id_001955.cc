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
//<ID> 1955
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
    cJSON *msg = NULL;
    cJSON *got_flag = NULL;
    cJSON *got_msg = NULL;
    char *msg_val = NULL;
    cJSON_bool added_flag = 0;
    cJSON_bool added_msg = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    flag = cJSON_CreateFalse();
    msg = cJSON_CreateString("example");
    added_flag = cJSON_AddItemToObject(root, "enabled", flag);
    added_msg = cJSON_AddItemToObject(root, "text", msg);

    // step 3: Core operations
    got_flag = cJSON_GetObjectItem(root, "enabled");
    got_msg = cJSON_GetObjectItem(root, "text");
    msg_val = cJSON_GetStringValue(got_msg);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(flag != NULL);
    validation ^= (int)(msg != NULL);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(added_msg != 0);
    validation ^= (int)(got_flag != NULL);
    validation ^= (int)(got_msg != NULL);
    validation ^= (int)(msg_val != NULL);
    validation ^= (int)(msg_val[0] == 'e');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}