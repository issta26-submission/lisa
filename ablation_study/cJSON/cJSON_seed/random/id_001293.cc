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
//<ID> 1293
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
    const char *name_str = (const char *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string into a root object
    root = cJSON_Parse("{\"name\": \"test\", \"count\": 5}");

    // step 3: Configure - retrieve the "name" field from the parsed object and get its string value
    name_item = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(name_item);

    // step 4: Operate - add a numeric field to the root and produce a printed representation
    cJSON_AddNumberToObject(root, "added", 3.14159);
    printed = cJSON_Print(root);

    // step 5: Validate - compute a simple validation score to ensure values flowed between calls
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(name_str != (const char *)0) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free the printed buffer and delete the root to release all resources
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}