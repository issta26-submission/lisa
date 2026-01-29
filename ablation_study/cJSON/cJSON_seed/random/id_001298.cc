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
//<ID> 1298
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
    cJSON *added_num = (cJSON *)0;
    char *name_value = (char *)0;
    char *printed = (char *)0;
    double number_to_add = 77.77;
    int validation_score = 0;

    // step 2: Initialize - parse JSON with a name field
    root = cJSON_Parse("{\"name\":\"example\"}");

    // step 3: Configure - retrieve the name item from the parsed root
    name_item = cJSON_GetObjectItem(root, "name");

    // step 4: Operate - extract the string value, add a numeric field, and obtain a printed representation
    name_value = cJSON_GetStringValue(name_item);
    added_num = cJSON_AddNumberToObject(root, "score", number_to_add);
    printed = cJSON_Print(root);

    // step 5: Validate - compute a simple validation score to ensure flow through APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(name_value != (char *)0) + (int)(added_num != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free the printed buffer and delete parsed root
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}