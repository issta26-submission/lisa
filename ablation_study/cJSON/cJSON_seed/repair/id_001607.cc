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
//<ID> 1607
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *flag_false = cJSON_CreateBool((cJSON_bool)0);

    // step 2: Configure
    cJSON_AddItemToObject(root, "isEnabled", flag_true);
    cJSON_AddItemToObject(root, "isVisible", flag_false);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    const char *version = cJSON_Version();
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    buffer[0] = printed[0];
    buffer[1] = version[0];
    buffer[2] = (char)('0' + (int)cJSON_IsTrue(flag_true));
    buffer[3] = (char)('0' + (int)cJSON_IsTrue(flag_false));
    buffer[4] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}