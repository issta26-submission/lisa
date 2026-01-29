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
//<ID> 410
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *tmp_buf = (char *)cJSON_malloc(64);
    memset(tmp_buf, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag_true = cJSON_CreateBool(1);
    cJSON *flag_false = cJSON_CreateBool(0);
    cJSON *retrieved_enabled = NULL;
    char *printed = NULL;
    cJSON_bool is_invalid = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "enabled", flag_true);
    cJSON_AddItemToObject(root, "settings", child);
    cJSON_AddItemToObject(child, "active", flag_false);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    retrieved_enabled = cJSON_GetObjectItem(root, "enabled");
    is_invalid = cJSON_IsInvalid(retrieved_enabled);
    tmp_buf[0] = printed ? printed[0] : '\0';
    tmp_buf[1] = (char)('0' + (int)is_invalid);
    tmp_buf[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}