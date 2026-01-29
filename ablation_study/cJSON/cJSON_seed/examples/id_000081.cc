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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 2: Configure
    size_t buflen = 6;
    char *buf = (char*)cJSON_malloc(buflen);
    memset(buf, 0, buflen);
    buf[0] = 'w';
    buf[1] = 'o';
    buf[2] = 'r';
    buf[3] = 'l';
    buf[4] = 'd';
    buf[5] = '\0';
    cJSON *target = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "target", target);
    cJSON *dup = cJSON_Duplicate(root, 1);

    // step 3: Operate & Validate
    cJSON_bool has_greeting = cJSON_HasObjectItem(dup, "greeting");
    cJSON_bool has_target = cJSON_HasObjectItem(dup, "target");
    cJSON *status = cJSON_CreateObject();
    cJSON_AddItemToObject(status, "greeting", cJSON_CreateBool(has_greeting));
    cJSON_AddItemToObject(status, "target", cJSON_CreateBool(has_target));
    cJSON_AddItemToObject(root, "status", status);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_free(buf);

    // API sequence test completed successfully
    return 66;
}