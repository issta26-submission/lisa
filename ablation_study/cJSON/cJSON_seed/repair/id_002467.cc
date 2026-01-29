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
//<ID> 2467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *t = cJSON_AddTrueToObject(child, "enabled");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *off = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "off", off);
    cJSON_AddTrueToObject(root, "rootflag");

    // step 2: Configure
    char *printed = cJSON_Print(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    cJSON_bool off_is_false = cJSON_IsFalse(off);
    buffer[1] = (char)(int)off_is_false;
    buffer[4] = (char)(int)cJSON_IsFalse(t);

    // step 3: Operate
    char *printed2 = cJSON_Print(child);
    buffer[2] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_AddItemToObject(child, "extra", cJSON_CreateObject());
    char *printed3 = cJSON_Print(root);
    buffer[3] = printed3 && printed3[0] ? printed3[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(printed3);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}