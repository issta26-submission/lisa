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
//<ID> 2468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_CreateObject();
    cJSON *enabled = cJSON_AddTrueToObject(settings, "enabled");
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *answer = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(root, "answer", answer);

    // step 2: Configure
    char *printed = cJSON_Print(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    cJSON *retrieved_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *retrieved_enabled = cJSON_GetObjectItem(retrieved_settings, "enabled");
    cJSON_bool is_false = cJSON_IsFalse(retrieved_enabled);

    // step 3: Operate
    buffer[1] = (char)(int)is_false;
    double ans_val = cJSON_GetNumberValue(answer);
    buffer[2] = (char)(int)ans_val;
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *extra = cJSON_CreateString("added");
    cJSON_AddItemToObject(dup, "note", extra);
    char *printed_dup = cJSON_Print(dup);

    // step 4: Validate and Cleanup
    buffer[3] = printed_dup && printed_dup[0] ? printed_dup[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}