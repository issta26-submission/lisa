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
//<ID> 1292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = NULL;
    cJSON *name_item = NULL;
    cJSON *age_item = NULL;
    const char *name_str = NULL;
    char *printed = NULL;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string into a root object
    root = cJSON_Parse("{\"name\":\"alice\",\"value\":123}");

    // step 3: Configure - add a numeric field to the parsed object
    age_item = cJSON_AddNumberToObject(root, "age", 42.0);

    // step 4: Operate - retrieve the name property and obtain its string value
    name_item = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(name_item);

    // step 5: Operate continued - serialize the modified object to a string
    printed = cJSON_Print(root);

    // step 6: Validate and Cleanup - compute a simple validation score, free allocations, and delete root
    validation_score = (int)(root != NULL) + (int)(name_item != NULL) + (int)(name_str != NULL) + (int)(age_item != NULL) + (int)(printed != NULL);
    (void)validation_score;
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}