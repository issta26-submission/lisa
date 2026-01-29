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
//<ID> 1183
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
    cJSON *fetched_name = (cJSON *)0;
    char *name_value = (char *)0;
    char *serialized = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - create root object and a string item
    root = cJSON_CreateObject();
    name_item = cJSON_CreateString("example_name");

    // step 3: Configure - add the string item to the root object under a known key
    cJSON_AddItemToObject(root, "name", name_item);

    // step 4: Operate - retrieve the "name" member case-sensitively and obtain its C string
    fetched_name = cJSON_GetObjectItemCaseSensitive(root, "name");
    name_value = cJSON_GetStringValue(fetched_name);

    // step 5: Validate - serialize the root unformatted and derive a small validation score, then free the serialization
    serialized = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(fetched_name != (cJSON *)0) + (int)(name_value != (char *)0);
    cJSON_free((void *)serialized);

    // step 6: Cleanup - delete root which frees all attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    (void)validation_score;
    return 66;
}