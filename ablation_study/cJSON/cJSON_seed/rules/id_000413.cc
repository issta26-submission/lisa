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
//<ID> 413
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
    cJSON *bool_item = cJSON_CreateBool(1);
    cJSON *retrieved = NULL;
    char *printed = NULL;
    cJSON_bool add_result = 0;
    cJSON_bool invalid_check = 0;

    // step 2: Setup / Configure
    add_result = cJSON_AddItemToObject(root, "enabled", bool_item);
    printed = cJSON_PrintUnformatted(root);
    retrieved = cJSON_GetObjectItem(root, "enabled");

    // step 3: Operate / Validate
    invalid_check = cJSON_IsInvalid(retrieved);
    tmp_buf[0] = (char)('0' + (int)add_result);
    tmp_buf[1] = (char)('0' + (int)invalid_check);
    tmp_buf[2] = printed[0];
    tmp_buf[3] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(tmp_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}