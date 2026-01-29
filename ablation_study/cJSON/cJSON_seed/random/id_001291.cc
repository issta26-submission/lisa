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
//<ID> 1291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *person = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *score_item = (cJSON *)0;
    cJSON *retrieved_score = (cJSON *)0;
    const char *name_str = (const char *)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON string into a root object
    root = cJSON_Parse("{\"person\": {\"name\": \"Alice\", \"age\": 30}}");

    // step 3: Configure - retrieve the nested person object and its name item
    person = cJSON_GetObjectItem(root, "person");
    name_item = cJSON_GetObjectItem(person, "name");

    // step 4: Operate - extract the name string, add a numeric score to the person, and produce a printed JSON
    name_str = cJSON_GetStringValue(name_item);
    score_item = cJSON_AddNumberToObject(person, "score", 88.5);
    retrieved_score = cJSON_GetObjectItem(person, "score");
    printed = cJSON_Print(root);

    // step 5: Validate - combine simple presence checks into a validation score
    validation_score = (int)(root != (cJSON *)0) + (int)(person != (cJSON *)0) + (int)(name_str != (const char *)0) + (int)(score_item != (cJSON *)0) + (int)(retrieved_score != (cJSON *)0);
    (void)validation_score;

    // step 6: Cleanup - free printed string and delete the root to release all allocated cJSON nodes
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}