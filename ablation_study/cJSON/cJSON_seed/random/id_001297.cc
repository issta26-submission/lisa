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
//<ID> 1297
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *details = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *id_item = (cJSON *)0;
    char *serialized = (char *)0;
    char *name_str = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string into a cJSON root
    root = cJSON_Parse("{\"name\":\"example\",\"details\": {\"id\": 7}}");

    // step 3: Configure - produce a textual representation and add a numeric field
    serialized = cJSON_Print(root);
    cJSON_AddNumberToObject(root, "added", 3.14);

    // step 4: Operate - retrieve nested objects and extract the string value
    details = cJSON_GetObjectItem(root, "details");
    id_item = cJSON_GetObjectItem(details, "id");
    name_item = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(name_item);

    // step 5: Validate - simple aggregation to ensure values flowed through
    validation_score = (int)(root != (cJSON *)0) + (int)(details != (cJSON *)0) + (int)(id_item != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(name_str != (char *)0) + (int)(serialized != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free serialized buffer and delete the parsed structure
    cJSON_free(serialized);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}