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
//<ID> 1006
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
    cJSON_AddStringToObject(config, "name", "alpha");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(config, "name");
    char *set_ret = cJSON_SetValuestring(name, "beta");
    char *tmp = (char *)cJSON_malloc(7);
    tmp[0] = 'm'; tmp[1] = 'i'; tmp[2] = 'r'; tmp[3] = 'r'; tmp[4] = 'o'; tmp[5] = 'r'; tmp[6] = '\0';
    cJSON_AddStringToObject(root, "mirrored", tmp);
    cJSON_free(tmp);
    cJSON_AddStringToObject(root, "copied_from_set", set_ret);

    // step 3: Operate and Validate
    cJSON *got = cJSON_GetObjectItemCaseSensitive(root, "copied_from_set");
    cJSON_SetValuestring(got, "gamma");
    cJSON *validate = cJSON_GetObjectItemCaseSensitive(root, "mirrored");
    cJSON_SetValuestring(validate, "mirror_updated");

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}