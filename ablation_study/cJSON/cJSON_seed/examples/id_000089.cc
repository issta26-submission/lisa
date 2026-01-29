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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *greet = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greet);

    // step 2: Configure
    char *buf = (char *)cJSON_malloc(6);
    buf[0] = 'w';
    buf[1] = 'o';
    buf[2] = 'r';
    buf[3] = 'l';
    buf[4] = 'd';
    buf[5] = '\0';
    cJSON *world = cJSON_CreateString(buf);
    cJSON_AddItemToObject(root, "target", world);
    cJSON *greet_dup = cJSON_Duplicate(greet, 1);
    cJSON_AddItemToObject(root, "greeting_copy", greet_dup);

    // step 3: Operate & Validate
    cJSON_bool has_greeting = cJSON_HasObjectItem(root, "greeting");
    cJSON_bool has_target = cJSON_HasObjectItem(root, "target");
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToArray(flags, cJSON_CreateNumber((double)has_greeting));
    cJSON_AddItemToArray(flags, cJSON_CreateNumber((double)has_target));
    cJSON_AddItemToObject(root, "flags", flags);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}