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
//<ID> 414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *tmp_buf = (char *)cJSON_malloc(128);
    memset(tmp_buf, 0, 128);
    cJSON *root = cJSON_CreateObject();
    cJSON *bool_true = cJSON_CreateBool(1);
    cJSON *bool_false = cJSON_CreateBool(0);
    cJSON_bool invalid_before_true = cJSON_IsInvalid(bool_true);
    cJSON_bool invalid_before_false = cJSON_IsInvalid(bool_false);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_enabled = NULL;
    cJSON_bool invalid_after_enabled = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "enabled", bool_true);
    cJSON_AddItemToObject(root, "disabled", bool_false);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_enabled = cJSON_GetObjectItem(parsed, "enabled");
    invalid_after_enabled = cJSON_IsInvalid(retrieved_enabled);
    tmp_buf[0] = (char)('0' + (int)invalid_before_true);
    tmp_buf[1] = (char)('0' + (int)invalid_before_false);
    tmp_buf[2] = (char)('0' + (int)invalid_after_enabled);
    tmp_buf[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}