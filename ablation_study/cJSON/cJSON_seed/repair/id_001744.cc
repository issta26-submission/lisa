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
//<ID> 1744
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *user = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "user", user);
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(user, "name", name);
    cJSON *score = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(user, "score", score);
    cJSON_AddTrueToObject(user, "verified");
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "note", "initial");

    // step 2: Configure
    double new_score = cJSON_SetNumberHelper(score, 99.5);
    cJSON_AddStringToObject(root, "status", "configured");

    // step 3: Operate and Validate
    cJSON *meta_ptr = cJSON_GetObjectItem(root, "meta");
    cJSON *detached_meta = cJSON_DetachItemViaPointer(root, meta_ptr);
    cJSON *detached_user = cJSON_DetachItemFromObject(root, "user");
    cJSON_bool meta_invalid = cJSON_IsInvalid(detached_meta);
    cJSON_bool user_invalid = cJSON_IsInvalid(detached_user);
    const char *ver = cJSON_Version();
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = ver ? ver[0] : 'V';
    buffer[2] = (char)('0' + (int)meta_invalid);
    buffer[3] = (char)('0' + (int)user_invalid);
    buffer[4] = (char)('0' + ((int)new_score % 10));
    (void)buffer;
    (void)new_score;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(detached_meta);
    cJSON_Delete(detached_user);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}