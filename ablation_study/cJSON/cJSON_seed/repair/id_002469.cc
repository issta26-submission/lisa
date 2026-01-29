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
//<ID> 2469
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
    cJSON *inner_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(child, "inner_false", inner_false);
    cJSON_AddItemToObject(child, "message", cJSON_CreateString("hello"));
    cJSON_AddItemToObject(root, "child", child);
    cJSON *root_true = cJSON_AddTrueToObject(root, "root_flag");

    // step 2: Configure
    char *printed1 = cJSON_Print(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed1 && printed1[0] ? printed1[0] : '\0';
    buffer[1] = root_true && root_true->type ? (char)1 : (char)0;

    // step 3: Operate
    cJSON_bool is_false = cJSON_IsFalse((const cJSON *const)inner_false);
    buffer[2] = (char)(int)is_false;
    cJSON_AddItemToObject(root, "status", cJSON_CreateString(is_false ? "false" : "not_false"));
    char *printed2 = cJSON_Print(root);
    buffer[3] = printed2 && printed2[0] ? printed2[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed1);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}