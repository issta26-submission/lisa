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
//<ID> 1681
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *msg = cJSON_CreateString("embedded message");
    cJSON *flag = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToObject(root, "message", msg);
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate and Validate
    cJSON *retr_msg = cJSON_GetObjectItem(root, "message");
    cJSON *retr_flag = cJSON_GetObjectItem(root, "flag");
    cJSON_bool is_invalid = cJSON_IsInvalid(retr_msg);
    cJSON_bool is_true = cJSON_IsTrue(retr_flag);
    char *printed = cJSON_Print(root);
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    const char *strval = cJSON_GetStringValue(retr_msg);
    buffer[1] = strval[0];
    buffer[2] = (char)('0' + (int)is_invalid);
    buffer[3] = (char)('0' + (int)is_true);
    (void)buffer;
    (void)strval;
    (void)retr_msg;
    (void)retr_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}