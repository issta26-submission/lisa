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
//<ID> 1185
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    char *serialized = (char *)0;
    char *name_value = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and add a string member
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "fuzz_target");

    // step 3: Operate - retrieve the name item case-sensitively and get its string value
    name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_value = cJSON_GetStringValue(name_item);

    // step 4: Validate - derive a simple validation score from the retrieved string and serialize
    validation_score = (int)name_value[0] + (int)name_item->valueint;
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 5: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}