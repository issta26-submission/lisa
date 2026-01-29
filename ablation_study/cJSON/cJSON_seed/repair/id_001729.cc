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
//<ID> 1729
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
    cJSON *score = cJSON_CreateNumber(1.0);
    cJSON_AddItemToObject(user, "score", score);
    cJSON_AddNullToObject(user, "reserved");
    cJSON *verified = cJSON_CreateTrue();
    cJSON_AddItemToObject(user, "verified", verified);

    // step 2: Configure
    double new_score = cJSON_SetNumberHelper(score, 42.5);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "configured");

    // step 3: Operate and Validate
    cJSON *retr_user = cJSON_GetObjectItem(root, "user");
    cJSON *retr_name = cJSON_GetObjectItem(retr_user, "name");
    const char *name_val = cJSON_GetStringValue(retr_name);
    cJSON *retr_verified = cJSON_GetObjectItem(retr_user, "verified");
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(64);
    memset(buffer, 0, 64);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name_val ? name_val[0] : 'Y';
    buffer[2] = (char)('0' + ((int)new_score % 10));
    buffer[3] = (char)('0' + cJSON_IsTrue(retr_verified));
    (void)buffer;
    (void)new_score;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}