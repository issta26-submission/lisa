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
//<ID> 1957
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
    cJSON *status_false = NULL;
    cJSON *message_str = NULL;
    cJSON *got_message = NULL;
    cJSON *got_status = NULL;
    char *got_message_val = NULL;
    cJSON_bool added_status = 0;
    cJSON_bool added_message = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    status_false = cJSON_CreateFalse();
    message_str = cJSON_CreateString("hello");
    added_status = cJSON_AddItemToObject(root, "status", status_false);
    added_message = cJSON_AddItemToObject(root, "message", message_str);

    // step 3: Core operations
    got_message = cJSON_GetObjectItem(root, "message");
    got_status = cJSON_GetObjectItem(root, "status");
    got_message_val = cJSON_GetStringValue(got_message);
    buf[0] = (got_message_val != NULL) ? got_message_val[0] : '\0';

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(status_false != NULL);
    validation ^= (int)(message_str != NULL);
    validation ^= (int)(added_status != 0);
    validation ^= (int)(added_message != 0);
    validation ^= (int)(got_message != NULL);
    validation ^= (int)(got_status != NULL);
    validation ^= (int)(got_message_val != NULL);
    validation ^= (int)(buf[0] == 'h');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}