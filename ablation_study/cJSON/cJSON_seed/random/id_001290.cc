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
//<ID> 1290
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *user = (cJSON *)0;
    cJSON *name_item = (cJSON *)0;
    cJSON *score_item = (cJSON *)0;
    char *name_str = (char *)0;
    char *name_copy = (char *)0;
    size_t name_len = 0;
    double score_value = 0.0;
    int validation_score = 0;

    // step 2: Initialize - parse a JSON with a nested object containing a string
    root = cJSON_Parse("{\"user\": {\"name\": \"Alice\"}}");

    // step 3: Configure - retrieve nested object and its name field
    user = cJSON_GetObjectItem(root, "user");
    name_item = cJSON_GetObjectItem(user, "name");

    // step 4: Operate - obtain the string, duplicate it using cJSON_malloc, compute a numeric score and add it to the root
    name_str = cJSON_GetStringValue(name_item);
    name_len = name_str ? strlen(name_str) : 0;
    name_copy = (char *)cJSON_malloc(name_len + 1);
    if (name_copy) memcpy(name_copy, name_str ? name_str : "", name_len + 1);
    score_value = name_str ? (double)name_str[0] : 0.0;
    cJSON_AddNumberToObject(root, "score", score_value);

    // step 5: Validate - retrieve the added number and compute a simple validation score
    score_item = cJSON_GetObjectItem(root, "score");
    validation_score = (int)(root != (cJSON *)0) + (int)(user != (cJSON *)0) + (int)(name_item != (cJSON *)0) + (int)(score_item != (cJSON *)0) + (int)(score_value == (name_str ? (double)name_str[0] : 0.0));
    (void)validation_score;

    // step 6: Cleanup - free duplicated string and delete the parsed root to release memory
    cJSON_free((void *)name_copy);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}