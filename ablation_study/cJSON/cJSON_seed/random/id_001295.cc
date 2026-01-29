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
//<ID> 1295
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
    cJSON *age_item = (cJSON *)0;
    const char *name_str = (const char *)0;
    char *printed = (char *)0;
    double age_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string containing a name
    root = cJSON_Parse("{\"name\":\"Alice\"}");

    // step 3: Configure - add a numeric field to the parsed object
    cJSON_AddNumberToObject(root, "age", 30.0);

    // step 4: Operate - retrieve items and extract values
    name_item = cJSON_GetObjectItem(root, "name");
    age_item = cJSON_GetObjectItem(root, "age");
    name_str = cJSON_GetStringValue(name_item);
    age_val = cJSON_GetNumberValue(age_item);

    // step 5: Validate - produce a printed representation and compute a simple score
    printed = cJSON_Print(root);
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(age_item != (cJSON *)0) + (int)(name_str != (const char *)0) + (int)(age_val == 30.0) + (int)(printed != (char *)0);
    (void)validation_score;

    // step 6: Cleanup - free allocated string and delete parsed structure
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}