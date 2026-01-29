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
//<ID> 415
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON *flag = cJSON_CreateBool(1);
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *retrieved_child = NULL;
    cJSON *retrieved_flag = NULL;
    cJSON_bool was_invalid = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "child", child);
    cJSON_AddItemToObject(child, "flag", flag);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    parsed = cJSON_Parse(printed);
    retrieved_child = cJSON_GetObjectItem(parsed, "child");
    retrieved_flag = cJSON_GetObjectItem(retrieved_child, "flag");
    was_invalid = cJSON_IsInvalid(retrieved_flag);
    buffer[0] = printed[0];
    buffer[1] = (char)('0' + (int)was_invalid);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}