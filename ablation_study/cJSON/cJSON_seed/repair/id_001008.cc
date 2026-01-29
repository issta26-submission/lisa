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
//<ID> 1008
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON_AddStringToObject(config, "mode", "initial");
    cJSON_AddStringToObject(config, "label", "alpha");

    // step 3: Operate and Validate
    cJSON *mode_item = cJSON_GetObjectItemCaseSensitive(config, "mode");
    cJSON_SetValuestring(mode_item, "updated");
    cJSON_AddStringToObject(root, "mode_copy", cJSON_GetStringValue(mode_item));
    char *temp = (char*)cJSON_malloc(8);
    memset(temp, 0, 8);
    temp[0] = 't';
    temp[1] = 'm';
    temp[2] = 'p';
    cJSON_free(temp);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}