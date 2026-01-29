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
//<ID> 1625
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed_retrieved = (char *)0;
    char *printed_root = (char *)0;
    cJSON_bool added = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_item = cJSON_CreateString("hello world");

    // step 3: Configure
    added = cJSON_AddItemToObject(root, "greeting", str_item);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "greeting");
    printed_retrieved = cJSON_PrintUnformatted(retrieved);
    printed_root = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added
        + (int)(retrieved != (cJSON *)0)
        + (int)(printed_retrieved != (char *)0)
        + (int)(printed_root != (char *)0);

    // step 6: Cleanup
    cJSON_free(printed_retrieved);
    cJSON_free(printed_root);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved;
    (void)str_item;
    (void)added;
    // API sequence test completed successfully
    return 66;
}