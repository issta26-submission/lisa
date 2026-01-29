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
//<ID> 1622
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *str_a = (cJSON *)0;
    cJSON *str_b = (cJSON *)0;
    cJSON *got_a = (cJSON *)0;
    cJSON *got_b = (cJSON *)0;
    char *printed = (char *)0;
    const char *val_a = (const char *)0;
    const char *val_b = (const char *)0;
    cJSON_bool added_a = 0;
    cJSON_bool added_b = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    str_a = cJSON_CreateString("hello");
    str_b = cJSON_CreateString("world");

    // step 3: Configure
    added_a = cJSON_AddItemToObject(root, "greeting", str_a);
    added_b = cJSON_AddItemToObject(root, "target", str_b);

    // step 4: Operate
    got_a = cJSON_GetObjectItem(root, "greeting");
    got_b = cJSON_GetObjectItem(root, "target");
    val_a = cJSON_GetStringValue(got_a);
    val_b = cJSON_GetStringValue(got_b);
    printed = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added_a
        + (int)added_b
        + (int)(got_a != (cJSON *)0)
        + (int)(got_b != (cJSON *)0)
        + (int)(val_a != (const char *)0)
        + (int)(val_b != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)val_a;
    (void)val_b;
    (void)got_a;
    (void)got_b;
    (void)str_a;
    (void)str_b;
    // API sequence test completed successfully
    return 66;
}