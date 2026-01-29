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
//<ID> 2108
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
    cJSON *msg_item = NULL;
    cJSON *true_item = NULL;
    cJSON *dup = NULL;
    cJSON *tmp = NULL;
    char *printed = NULL;
    char *got_str = NULL;
    int validation = 0;

    // step 2: Setup
    root = cJSON_CreateObject();
    msg_item = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "msg", msg_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", true_item);

    // step 3: Operate
    dup = cJSON_Duplicate(root, 1);
    cJSON_AddStringToObject(dup, "status", "copied");
    tmp = cJSON_GetObjectItem(dup, "msg");
    got_str = cJSON_GetStringValue(tmp);
    printed = cJSON_PrintUnformatted(dup);

    // step 4: Validate
    validation ^= (int)(root != NULL);
    validation ^= (int)(msg_item != NULL);
    validation ^= (int)(true_item != NULL);
    validation ^= (int)(dup != NULL);
    validation ^= (int)(tmp != NULL);
    validation ^= (int)(got_str != NULL);
    validation ^= (int)(got_str[0] == 'h');
    validation ^= (int)(printed != NULL);
    buf[0] = (char)(validation + '0');

    // step 5: Cleanup
    cJSON_free(printed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}