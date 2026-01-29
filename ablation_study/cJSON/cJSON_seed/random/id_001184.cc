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
//<ID> 1184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *serialized = (char *)0;
    const char *name_value = (const char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and add a string member
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "name", "example_name");

    // step 3: Operate - retrieve the "name" item case-sensitively and obtain its string value
    retrieved = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_value = cJSON_GetStringValue(retrieved);

    // step 4: Validate - compute a simple validation score and serialize the object unformatted
    validation_score = (int)(name_value != (const char *)0);
    serialized = cJSON_PrintUnformatted(root);
    cJSON_free((void *)serialized);

    // step 5: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}