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
//<ID> 1626
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *item1 = (cJSON *)0;
    cJSON *item2 = (cJSON *)0;
    cJSON *fetched1 = (cJSON *)0;
    cJSON *fetched2 = (cJSON *)0;
    char *printed1 = (char *)0;
    char *printed2 = (char *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    item1 = cJSON_CreateString("hello");
    item2 = cJSON_CreateString("world");

    // step 3: Configure
    added1 = cJSON_AddItemToObject(root, "greeting", item1);
    added2 = cJSON_AddItemToObject(root, "noun", item2);

    // step 4: Operate
    fetched1 = cJSON_GetObjectItem(root, "greeting");
    fetched2 = cJSON_GetObjectItem(root, "noun");
    printed1 = cJSON_PrintUnformatted(fetched1);
    printed2 = cJSON_PrintUnformatted(fetched2);

    // step 5: Validate
    validation_score = (int)added1
        + (int)added2
        + (int)(fetched1 != (cJSON *)0)
        + (int)(fetched2 != (cJSON *)0)
        + (int)(printed1 != (char *)0)
        + (int)(printed2 != (char *)0);

    // step 6: Cleanup
    if (printed1) { cJSON_free(printed1); }
    if (printed2) { cJSON_free(printed2); }
    cJSON_Delete(root);
    (void)validation_score;
    // API sequence test completed successfully
    return 66;
}