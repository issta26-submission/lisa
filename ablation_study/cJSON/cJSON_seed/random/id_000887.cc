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
//<ID> 887
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const char *greeting = "hello, cJSON";
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    cJSON_bool added_string = (cJSON_bool)0;
    char *printed = (char *)0;
    int validation_score = 0;

    // step 2: Initialize / Configure
    root = cJSON_CreateObject();

    // step 3: Operate (create string and add items using required APIs)
    str_item = cJSON_CreateString(greeting);
    added_string = cJSON_AddItemToObject(root, "greeting", str_item);
    true_item = cJSON_AddTrueToObject(root, "enabled");
    num_item = cJSON_AddNumberToObject(root, "answer", 42.0);

    // step 4: Validate by printing and aggregating simple checks
    printed = cJSON_PrintUnformatted(root);
    validation_score = (int)(root != (cJSON *)0) + (int)added_string + (int)(str_item != (cJSON *)0) + (int)(true_item != (cJSON *)0) + (int)(num_item != (cJSON *)0) + (int)(printed != (char *)0);
    (void)validation_score;
    if (printed) { cJSON_free(printed); }

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}