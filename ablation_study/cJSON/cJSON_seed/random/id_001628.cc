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
//<ID> 1628
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *val1 = (cJSON *)0;
    cJSON *val2 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    cJSON *fetched2 = (cJSON *)0;
    char *printed = (char *)0;
    const char *greeting_str = (const char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    val1 = cJSON_CreateString("hello");
    val2 = cJSON_CreateString("world");

    // step 3: Configure
    added1 = cJSON_AddItemToObject(root, "greeting", val1);
    added2 = cJSON_AddItemToObject(root, "target", val2);

    // step 4: Operate
    fetched1 = cJSON_GetObjectItem(root, "greeting");
    fetched2 = cJSON_GetObjectItem(root, "target");
    printed = cJSON_PrintUnformatted(root);
    greeting_str = cJSON_GetStringValue(fetched1);

    // step 5: Validate
    validation_score = (int)added1
        + (int)added2
        + (int)(fetched1 != (cJSON *)0)
        + (int)(fetched2 != (cJSON *)0)
        + (int)(greeting_str != (const char *)0)
        + (int)(printed != (char *)0);

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);
    (void)validation_score;
    (void)greeting_str;
    (void)fetched2;
    (void)fetched1;
    (void)val2;
    (void)val1;
    // API sequence test completed successfully
    return 66;
}