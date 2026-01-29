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
//<ID> 1747
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child_a = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(root, "a", child_a);
    cJSON *child_b = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(root, "b", child_b);
    cJSON_AddBoolToObject(root, "flag", 1);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta, "note", note);
    double updated = cJSON_SetNumberHelper(child_b, 2.718);

    // step 3: Operate and Validate
    cJSON *got_b = cJSON_GetObjectItem(root, "b");
    cJSON *detached_a = cJSON_DetachItemFromObject(root, "a");
    cJSON *detached_b = cJSON_DetachItemViaPointer(root, got_b);
    char *printed = cJSON_PrintUnformatted(root);
    const char *ver = cJSON_Version();
    cJSON_bool invalid_flag = cJSON_IsInvalid(detached_a);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver ? ver[0] : 'V';
    buffer[2] = (char)('0' + (int)invalid_flag);
    (void)updated;
    (void)buffer;
    (void)ver;
    (void)invalid_flag;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_a);
    cJSON_Delete(detached_b);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}