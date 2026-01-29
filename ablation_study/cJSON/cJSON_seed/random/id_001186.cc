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
//<ID> 1186
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *config = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *fetched_config = (cJSON *)0;
    cJSON *fetched_name = (cJSON *)0;
    char *name_str = (char *)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root and a nested config object
    root = cJSON_CreateObject();
    config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 3: Configure - add a name string and a version number into config
    name_item = cJSON_CreateString("tester");
    cJSON_AddItemToObject(config, "name", name_item);
    cJSON_AddNumberToObject(config, "version", 1.23);

    // step 4: Operate - retrieve nested object and its name member case-sensitively and get the string value
    fetched_config = cJSON_GetObjectItemCaseSensitive(root, "config");
    fetched_name = cJSON_GetObjectItemCaseSensitive(fetched_config, "name");
    name_str = cJSON_GetStringValue(fetched_name);

    // step 5: Validate - serialize unformatted and compute a simple validation score, free the serialization
    serialized = cJSON_PrintUnformatted(root);
    validation_score = (int)(name_str != (char *)0) + (int)(serialized != (char *)0);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}