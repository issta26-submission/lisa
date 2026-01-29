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
//<ID> 1623
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *s1 = (cJSON *)0;
    cJSON *s2 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    cJSON *got2 = (cJSON *)0;
    char *out = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    s1 = cJSON_CreateString("hello");
    s2 = cJSON_CreateString("world");

    // step 3: Configure
    added1 = cJSON_AddItemToObject(root, "greeting", s1);
    added2 = cJSON_AddItemToObject(root, "target", s2);

    // step 4: Operate
    got1 = cJSON_GetObjectItem(root, "greeting");
    got2 = cJSON_GetObjectItem(root, "target");
    out = cJSON_PrintUnformatted(root);

    // step 5: Validate
    validation_score = (int)added1
        + (int)added2
        + (int)(got1 != (cJSON *)0)
        + (int)(got2 != (cJSON *)0)
        + (int)(out != (char *)0);

    // step 6: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    (void)validation_score;
    (void)got1;
    (void)got2;
    (void)added1;
    (void)added2;
    // API sequence test completed successfully
    return 66;
}