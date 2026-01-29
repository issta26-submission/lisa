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
//<ID> 1294
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
    cJSON *added_num = (cJSON *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON object with a string field
    root = cJSON_Parse("{\"name\": \"alpha\", \"value\": 123}");

    // step 3: Configure - retrieve the "name" field from the parsed root
    name_item = cJSON_GetObjectItem(root, "name");

    // step 4: Operate - extract the string value and add a new numeric field to the root
    name_str = cJSON_GetStringValue(name_item);
    added_num = cJSON_AddNumberToObject(root, "added", 3.1415);

    // step 5: Validate - print the modified JSON to a heap buffer and compute a simple score
    printed = cJSON_Print(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(name_str != (const char *)0) + (int)(added_num != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free the printed buffer and delete the root (which frees child items)
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}