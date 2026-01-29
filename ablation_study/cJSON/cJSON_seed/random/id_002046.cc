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
//<ID> 2046
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    char buf[16];
    memset(buf, 0, sizeof(buf));
    cJSON *root = NULL;
    cJSON *str = NULL;
    cJSON *bool_true = NULL;
    cJSON *added_bool_item = NULL;
    cJSON *msg_item = NULL;
    char *got_str = NULL;
    void *mem = NULL;
    char *copied = NULL;
    cJSON_bool added_str_flag = 0;
    cJSON_bool invalid_flag = 0;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    str = cJSON_CreateString("greeting");
    bool_true = cJSON_CreateTrue();

    // step 3: Configure
    added_bool_item = cJSON_AddBoolToObject(root, "flag", 1);
    added_str_flag = cJSON_AddItemToObject(root, "message", str);

    // step 4: Operate
    msg_item = cJSON_GetObjectItem(root, "message");
    got_str = cJSON_GetStringValue(msg_item);
    invalid_flag = cJSON_IsInvalid(bool_true);
    mem = cJSON_malloc((got_str ? (strlen(got_str) + 1) : 1));
    copied = (char *)mem;
    if (got_str && copied) memcpy(copied, got_str, strlen(got_str) + 1);

    // step 5: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(str != NULL);
    validation ^= (int)(bool_true != NULL);
    validation ^= (int)(added_bool_item != NULL);
    validation ^= (int)(added_str_flag != 0);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(invalid_flag == 0);
    validation ^= (int)(copied != NULL);
    validation ^= (int)(copied && copied[0] == 'g');
    buf[0] = (char)(validation + '0');

    // step 6: Cleanup
    cJSON_free(mem);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}