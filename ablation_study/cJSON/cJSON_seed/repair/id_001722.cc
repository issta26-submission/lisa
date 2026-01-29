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
//<ID> 1722
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
    cJSON *score = cJSON_CreateNumber(0.0);
    cJSON_AddItemToObject(user, "score", score);
    cJSON *active = cJSON_CreateTrue();
    cJSON_AddItemToObject(user, "active", active);
    cJSON_AddNullToObject(user, "optional");

    // step 2: Configure
    double new_score = cJSON_SetNumberHelper(score, 88.5);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddStringToObject(meta, "status", "configured");

    // step 3: Operate and Validate
    cJSON_bool is_active = cJSON_IsTrue(active);
    char *printed = cJSON_PrintUnformatted(root);
    char *buffer = (char *)cJSON_malloc(128);
    memset(buffer, 0, 128);
    buffer[0] = printed ? printed[0] : 'X';
    buffer[1] = name->valuestring ? name->valuestring[0] : 'Y';
    buffer[2] = (char)('0' + (score->valueint % 10));
    buffer[3] = is_active ? 'T' : 'F';
    (void)new_score;
    (void)buffer;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}