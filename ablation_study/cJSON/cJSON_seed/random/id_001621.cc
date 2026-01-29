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
//<ID> 1621
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *greeting = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool added = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    greeting = cJSON_CreateString("hello world");

    // step 3: Configure
    added = cJSON_AddItemToObject(root, "greeting", greeting);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "greeting");
    printed = cJSON_PrintUnformatted(retrieved);

    // step 5: Validate
    validation_score = (int)added + (int)(retrieved != (cJSON *)0) + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_Delete(root);
    cJSON_free(printed);
    (void)validation_score;
    (void)added;
    (void)retrieved;
    (void)greeting;
    (void)root;
    // API sequence test completed successfully
    return 66;
}