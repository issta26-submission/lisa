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
//<ID> 1954
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
    cJSON *false_item = NULL;
    cJSON *got_message = NULL;
    char *msg = NULL;
    cJSON_bool added_flag = 0;
    int validation = 0;
    char first_char = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello");
    false_item = cJSON_CreateFalse();
    added_flag = cJSON_AddItemToObject(root, "flag", false_item);
    cJSON_AddItemToObject(root, "message", str_item);

    // step 3: Core operations
    got_message = cJSON_GetObjectItem(root, "message");
    msg = cJSON_GetStringValue(got_message);
    first_char = msg[0];

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str_item != NULL);
    validation ^= (int)(false_item != NULL);
    validation ^= (int)(added_flag != 0);
    validation ^= (int)(got_message != NULL);
    validation ^= (int)(msg != NULL);
    validation ^= (int)(first_char == 'h');
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_Delete(root);

    // step 6: Finalize
    // API sequence test completed successfully
    return 66;
}