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
//<ID> 95
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
    cJSON_AddItemToObject(root, "settings", settings);
    cJSON *name = cJSON_CreateString("initial");
    cJSON_AddItemToObject(settings, "name", name);
    cJSON *names = cJSON_CreateArray();
    cJSON_AddItemToObject(settings, "names", names);
    cJSON *n1 = cJSON_CreateString("alice");
    cJSON_AddItemToArray(names, n1);
    cJSON *n2 = cJSON_CreateString("bob");
    cJSON_AddItemToArray(names, n2);

    // step 2: Configure
    cJSON_SetValuestring(name, "carol");
    cJSON *dup_settings = cJSON_Duplicate(settings, 1);
    cJSON_AddItemToObject(root, "backup_settings", dup_settings);
    cJSON *ver_item = cJSON_AddNumberToObject(root, "version", 2.0);

    // step 3: Operate and Validate
    cJSON *got_settings = cJSON_GetObjectItem(root, "settings");
    cJSON *got_names = cJSON_GetObjectItem(got_settings, "names");
    int names_count = cJSON_GetArraySize(got_names);
    cJSON_AddNumberToObject(root, "names_count", (double)names_count);
    cJSON *got_name = cJSON_GetObjectItem(got_settings, "name");
    const char *name_str = cJSON_GetStringValue(got_name);
    cJSON_AddNumberToObject(root, "first_char_code", (double)(unsigned char)name_str[0]);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    (void)ver_item;

    return 66;
    // API sequence test completed successfully
}