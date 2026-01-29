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
//<ID> 1240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *inner = cJSON_CreateObject();

    // step 2: Configure
    cJSON_AddBoolToObject(inner, "enabled", 1);
    cJSON_AddItemToObject(root, "inner", inner);
    cJSON *inner_ref = cJSON_Duplicate(inner, 1);
    cJSON_AddItemToObject(root, "inner_ref", inner_ref);
    cJSON_AddBoolToObject(root, "root_flag", 0);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    const char *version = cJSON_Version();
    cJSON *got_inner = cJSON_GetObjectItem(root, "inner");
    cJSON *got_ref = cJSON_GetObjectItem(root, "inner_ref");
    (void)version;
    (void)got_ref;

    // step 4: Validate & Cleanup
    cJSON_AddBoolToObject(got_inner, "validated", 1);
    char *final_print = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_free(final_print);
    cJSON_Delete(root);
    return 66;
}