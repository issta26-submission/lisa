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
//<ID> 1299
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
    char *name_str = (char *)0;
    char *printed = (char *)0;
    double first_char_val = 0.0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string containing a name
    root = cJSON_Parse("{\"name\":\"Alice\",\"info\":{\"id\":123}}");

    // step 3: Configure - retrieve the name item and extract its string value
    name_item = cJSON_GetObjectItem(root, "name");
    name_str = cJSON_GetStringValue(name_item);

    // step 4: Operate - derive a numeric value from the name and add it to the object, then serialize
    first_char_val = (double)name_str[0];
    cJSON_AddNumberToObject(root, "first_char_code", first_char_val);
    printed = cJSON_Print(root);

    // step 5: Validate - compute a simple validation score based on successful steps
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(name_str != (char *)0) + (int)(printed != (char *)0) + (int)(first_char_val == (double)('A'));
    (void)validation_score;

    // step 6: Cleanup - free the printed buffer and delete the root object
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}