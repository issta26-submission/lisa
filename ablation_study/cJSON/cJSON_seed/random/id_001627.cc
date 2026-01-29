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
//<ID> 1627
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *retrieved = (cJSON *)0;
    char *printed = (char *)0;
    const char *retrieved_value = (const char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str1 = cJSON_CreateString("hello");
    str2 = cJSON_CreateString("world");

    // step 3: Configure
    added1 = cJSON_AddItemToObject(root, "first", str1);
    added2 = cJSON_AddItemToObject(root, "second", str2);

    // step 4: Operate
    retrieved = cJSON_GetObjectItem(root, "second");
    retrieved_value = cJSON_GetStringValue(retrieved);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)(root != (cJSON *)0)
        + (int)added1
        + (int)added2
        + (int)(retrieved != (cJSON *)0)
        + (int)(retrieved_value != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)retrieved_value;
    (void)retrieved;
    (void)str2;
    (void)str1;
    // API sequence test completed successfully
    return 66;
}