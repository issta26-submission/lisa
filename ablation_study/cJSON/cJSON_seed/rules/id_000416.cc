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
//<ID> 416
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
    cJSON *bool_true = cJSON_CreateBool(1);
    cJSON *bool_false = cJSON_CreateBool(0);
    cJSON_bool add_ok_root_true = 0;
    cJSON_bool add_ok_root_child = 0;
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_enabled = NULL;
    cJSON_bool retrieved_invalid = 0;

    // step 2: Setup / Configure
    add_ok_root_true = cJSON_AddItemToObject(root, "enabled", bool_true);
    add_ok_root_child = cJSON_AddItemToObject(root, "settings", child);
    cJSON_AddItemToObject(child, "verbose", bool_false);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_enabled = cJSON_GetObjectItem(parsed, "enabled");
    retrieved_invalid = cJSON_IsInvalid(retrieved_enabled);
    tmp_buf[0] = (char)('0' + (int)add_ok_root_true);
    tmp_buf[1] = (char)('0' + (int)add_ok_root_child);
    tmp_buf[2] = (char)('0' + (int)retrieved_invalid);
    tmp_buf[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}