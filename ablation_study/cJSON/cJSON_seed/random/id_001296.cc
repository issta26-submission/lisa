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
//<ID> 1296
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
    char *name_value = (char *)0;
    char *name_copy = (char *)0;
    int validation_score = 0;
    size_t name_len = 0;

    // step 2: Initialize - parse a JSON string with a name field
    root = cJSON_Parse("{\"name\": \"Alice\"}");

    // step 3: Configure - retrieve the name field and add an age number to the object
    name_item = cJSON_GetObjectItem(root, "name");
    cJSON_AddNumberToObject(root, "age", 30.0);
    age_item = cJSON_GetObjectItem(root, "age");

    // step 4: Operate - get the string value, make a heap copy, then free the copy
    name_value = cJSON_GetStringValue(name_item);
    name_len = strlen(name_value);
    name_copy = (char *)cJSON_malloc(name_len + 1);
    strcpy(name_copy, name_value);
    cJSON_free((void *)name_copy);

    // step 5: Validate - compute a simple validation score from pointers and content
    validation_score = (int)(root != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(age_item != (cJSON *)0) + (int)(name_value[0] == 'A');
    (void)validation_score;

    // step 6: Cleanup - delete the parsed root which frees attached items
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}