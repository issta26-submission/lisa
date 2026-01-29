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
//<ID> 1180
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
    name_item = cJSON_AddStringToObject(root, "name", "fuzz-test");

    // step 3: Operate - retrieve the string member case-sensitively and extract its C string
    name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_value = cJSON_GetStringValue(name_item);

    // step 4: Validate - serialize the JSON unformatted and compute a simple validation score
    serialized = cJSON_PrintUnformatted(root);
    validation_score = (int)(name_value != (char *)0) + (int)(serialized != (char *)0);

    // step 5: Cleanup - free serialized buffer and delete root (frees all children)
    cJSON_free((void *)serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)name_value;
    (void)validation_score;
    return 66;
}