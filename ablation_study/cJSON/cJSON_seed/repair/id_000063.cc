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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *settings = cJSON_AddObjectToObject(root, "settings");
    cJSON *mode = cJSON_CreateString("init");
    cJSON_AddItemToObject(settings, "mode", mode);

    // step 2: Configure
    char *updated_mode = cJSON_SetValuestring(mode, "active");
    cJSON_AddNumberToObject(settings, "threshold", 3.1415);
    cJSON_AddNumberToObject(root, "base", 42.0);

    // step 3: Operate and Validate
    cJSON_bool has_settings = cJSON_HasObjectItem(root, "settings");
    cJSON_bool has_mode = cJSON_HasObjectItem(settings, "mode");
    double validation_value = (double)has_settings * 100.0 + (double)has_mode * 10.0 + (double)(unsigned char)updated_mode[0];
    cJSON_AddNumberToObject(root, "validation", validation_value);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}